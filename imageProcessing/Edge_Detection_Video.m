%This function uses the video from the webcam and determines the location of where the
%circles are for the board game Gone Fishin' 

%Get time stamp working for csv code and export csv's as txt's 
function [centers] = Edge_Detection_Video()
%webcamlist;

cam = webcam('USB Video Device');
preview(cam)
%frames = snapshot(cam);
workingDir = tempname;
mkdir(workingDir)
mkdir(workingDir,'Image')
%FishVideo = VideoWriter('fish.avi');
centers = zeros(21,2); 
%vid = imread('Fish left dots.jpg');


%Assume there are fish in all spots of the cut outs for the board using the
%below website 
    % http://www.mathworks.com/help/images/examples/detect-and-measure-circular-objects-in-an-image.html?s_tid=gn_loc_drop
%Run this code only on the first image to test and see if it works 
v = VideoWriter('fish.avi','Uncompressed AVI');
open(v)

for i = 1:1
    %Gets the current frames snapshot from the camera 
    frames = snapshot(cam);
    %frames = vid; 
    %Opens the figure window 
    figure  
%Radius is approximately 160 to 170 pixels and use the d=imdistline to test
%below 
    %d = imdistline;
    %Find the centers and radii of each of the circles 
    [centers, radii] = imfindcircles(frames,[28 40], 'ObjectPolarity','dark','Sensitivity',0.92,'Method','twostage')
    %Show the frames with the drawn circle on the shown image 
    h = viscircles(centers, radii)
    imshow(frames) 
    hold on;
    viscircles(centers, radii);
    drawnow 
    %centers = centers; 
    %Taking the center locations and assigning them to radius points 
    %Write the frames to the video 
    imwrite(frames,['Image' int2str(i), '.jpg'])
    %Write the video 
    writeVideo(v,frames);
    %Clear the figure 
    %clf
end
%Closes writing to the video 
close(v)
%Takes the found center points of each of the circles and compares the color in the image taken from to which is the darkest
%Divide by 15 so that each pixel is one centimeter in the csv 
%centers = centers/8.5; 
centers = centers/16; 
RGB_values = zeros((size(centers,1)),3); 
%Loop through the centers values and get back the RGB values for each of
%the centers detected 
for i = 1:size(centers,1) 
    point = centers(i,1:2);
    redValue = frames(round(point(2)), round(point(1)), 1);
    greenValue = frames(round(point(2)), round(point(1)), 2);
    blueValue = frames(round(point(2)), round(point(1)), 3);
    RGB_values(i,1:3) = [redValue, greenValue, blueValue]; 
end 

sum_RGB = zeros(size(RGB_values,1),1); 
for i = 1:size(RGB_values,1)
    %Sum each row of the values 
    sum_RGB(i) = sum(RGB_values(i,1:3));  
end
 

%Calculate the distance between the side dot and the center dot 
%distance = sqrt((240 - small_closetofish_circle_location(1))^2 + (290-small_closetofish_circle_location(2))^2);
%New code will be in relation to the center of the boards location and
%finding a rough estimate of the distance being 12 mm between the center of
%the board and the side circle detected from the previous code 
%Find the lowest value in sum_RGB and then update the csv accordinlgy
[Minimum,Indice] = min(sum_RGB);
%Find the Indice in centers for where the circle is located 
replacement_location = round(centers(Indice,1:2)); 

%Find the small circle closest to the enemy to find the distance for it on 
% the left side of the board 
%[small_closetofish_circle_location] = Small_dot_detection_test(cam);
small_closetofish_circle_location = [224 272]; 

y_distance = 425; 
x_distance = 425; 
for s = 1:size(centers, 1) 
    %centers = centers - small_closetofish_circle_location; 
    centers(s,1) = (round(centers(s,1)) - small_closetofish_circle_location(1)) + x_distance;
    centers(s,2) = (round(centers(s,2)) - small_closetofish_circle_location(2)) + y_distance;
end 

%Origin coordinates to base the points off of 
small_closetofish_circle_location = ((small_closetofish_circle_location - small_closetofish_circle_location) + x_distance)/9.5; 


%Find the largest size of the centers to determine how big to make the csv 
%and replace them with all zeros  
max_size_csv = round(max(centers)); 
max_size_csv = zeros(max_size_csv);



%Write in zeros at all other spots available  
for j = 1:size(centers,1)
    filename1 = 'testcode/test/Fish.csv';
    %find the location of the found fish to replace with ones in the csv
    %file where row is x and col is y 
    row = round(centers(j,1)) + 1;
    col = round(centers(j,2)) + 1;
    %distance = distance/15; 
    %Replace the zero with one to show where the fish are 
    max_size_csv(row,col) = 1; 
    max_size_csv(round(small_closetofish_circle_location(1)), round(small_closetofish_circle_location(2))) = -1; 
    %Place a 2 where the darkest detected fish is
    max_size_csv(replacement_location(1), replacement_location(2)) = 2; 
end 

%Where the points of the circles are 
%max_size_csv(small_closetofish_circle_location(1),small_closetofish_circle_location(2)) = -1; 

%Write the matrix back in to the file to record the fish locations of found and not found  
csvwrite( filename1, max_size_csv); 

filename2 = 'FishText.txt';
fprintf( filename2, max_size_csv);
%Start over again! 






end 