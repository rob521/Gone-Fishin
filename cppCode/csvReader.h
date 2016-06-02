//
//  main.cpp
//  Test read in
//
//  Created by Samantha Hemleben on 5/18/16.
//  Copyright © 2016 Samantha Hemleben. All rights reserved.
//

#ifndef _CSVREADER_
#define _CSVREADER_

//#include "Test Read in.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <time.h>
#include <ctime>
#include<string.h>
#include<sstream>
#include<iterator>
#include"SerialClass.h"
#include"main.h"

using namespace std;
ifstream infile;
int size_of_array_one, size_of_array_two;
vector <vector <double>> import_array;
string strData = "";
vector <double> maximum;
int temp_location_x, temp_location_y, x_move, y_move;
vector<double> new_coordinate;
int x_origin = 15; // X was changed to 15
int y_origin = 12; // Y was changed to 12
int multiply_distance = 28; // There is 50 centimeters per step

struct location allLoc[50], center = {224, 272};
char delim = ',';
int spotiterator = 1;
int fish_attemps = 0;

// import data into the array import array
void readArray(){
    FILE *filept;
    filept = fopen(fileName, "r");
    //filept = fopen("Fish.txt", "r");
    if(filept == NULL){
        printf("Cannot read from CSV file\n");
        exit(1);
    }

    char inChar = ' ';
    int row = 0, column = 0, k = 0, c, neg;

    for(int i = 0; i < 50; i++)
        allLoc[i] = {0, 0};

    while(!feof(filept)){
        column = 0;
        inChar = fgetc(filept);
        while(inChar != '\n' && !feof(filept) && k < 50){
            if(inChar != ',' && inChar != '\n'){
                if(inChar == '-') inChar = fgetc(filept);
                else if(inChar != '0'){
                    allLoc[k].xAxis = row;
                    allLoc[k].yAxis = column;
                    k++;
                }
                column++;
            }
            inChar = fgetc(filept);
        }
        row++;
    }
    fclose(filept);

    cout <<"Available position:\n";
    for(int i = 0; i < k; i++)
        printf("x = %d,\ty = %d\n", allLoc[i].xAxis, allLoc[i].yAxis/* - csvOffset*/);
    cout <<"\n";
}

//Based on the new coordinate system tells the fishing rod where to move
int move_to(struct location *loc, int randInt){
    srand(randInt);
    int spot;
    //do{
        //spot = rand() % 50;
        spot = spotiterator;
        spotiterator++;
        if (allLoc[spotiterator].xAxis == 0 && allLoc[spotiterator].yAxis == 0) spotiterator = 0;
    //}while (allLoc[spot].xAxis == 0 && allLoc[spot].yAxis == 0);

    /*if(userTest){
        cout <<"spot: ";
        cin >>spot;
    }*/

    loc->xAxis = (allLoc[spot].xAxis - csvOffset) /2;
    (allLoc[spot].xAxis - csvOffset > center.xAxis) ? loc->xAxis = loc->xAxis * 1 : loc->xAxis = loc->xAxis * -1;
    loc->yAxis = (allLoc[spot].yAxis - csvOffset) * 1;
    (allLoc[spot].yAxis - csvOffset > center.yAxis) ? loc->yAxis = loc->yAxis * 1 : loc->yAxis = loc->yAxis * -1;
    fish_attemps++;
    cout <<"Location " <<spot << "\nMove this much in x " << loc->xAxis << " Move this much in y " << loc->yAxis << endl <<"fishing attempts = " <<fish_attemps <<endl;
    loc->xAxis = 0;//***************************************
    loc->yAxis = 0;//***************************************
    return 0;
}

#endif // _CSVREADER_
