#include "SerialClass.h"
#include "main.h"
#include "csvReader.h"
#include<iostream>
#include<unistd.h>
#include<time.h>


using namespace std;
int timmer;

void initialize(){
    readArray();
    if(!arduino.IsConnected()){
        cout<<"Connection failed on ";
        cout<<port;
        cout<< "\n";
        exit(1);
    }
}

void startWait(){
    char message[BUFFER];

    //printf("%s\n", message);
    //while(!memcmp(message, "start", sizeof(char)*5)){
    while(strstr(message, "start") ==  NULL){
        arduino.ReadData(message,BUFFER);
        //printf("%s\n", message);
        Sleep(500);
    }
    printf("start\n");
    timmer = time(NULL);
}

int waitFish(){
    //TODO: need to add code*********************
    //returns 1 when ready to fish
    if(userTest){
        cout<<"Press enter when ready";
        fflush(stdin);
        while(getchar() != '\n');
        //Sleep(5000);
        return 1;
    }
    srand(time(NULL));
    int temp = (rand() % 2000) + 8000;
    Sleep(temp);
    //Sleep(delaystabalize);
    return 1;
}

void dropoff(){
    char message[BUFFER];
    moveBot(stepper1, dropoffLoc.xAxis);
    Sleep(500);
    moveBot(stepper2, dropoffLoc.yAxis);
    Sleep(2000);
    sprintf(message, "servo1.down\n");
    arduino.WriteData(message, strlen(message));
    Sleep(500);
    sprintf(message, "servo2.shake\n");
    arduino.WriteData(message, strlen(message));
    Sleep(500);
}

void moveBot(int motor, int amount){
    char message[BUFFER], command[BUFFER];

    switch(motor){
        case stepper1:
            sprintf(message, "%s.%d\n", "stepper1", amount);
            break;
        case stepper2:
            sprintf(message, "%s.%d\n", "stepper2", amount);
            break;
        case servo1:
            (amount == up) ? sprintf(command, "%s", "up") : sprintf(command, "%s", "down");
            sprintf(message, "%s.%s\n", "servo1", command);
            break;
        case servo2:
            (amount == up) ? sprintf(command, "%s", "up") : sprintf(command, "%s", "down");
            sprintf(message, "%s.%s\n", "servo2", command);
            break;
    }

    while(!arduino.WriteData(message, strlen(message)));
}

void fish(){
    char message[BUFFER];
    sprintf(message, "servo2.down\n");
    arduino.WriteData(message, strlen(message));
    Sleep(1000);
    sprintf(message, "servo2.up\n");
    arduino.WriteData(message, strlen(message));
}

void fishingPond(struct location *entirePond, int sizeofpond, struct location *fishingSpot){
    srand(time(NULL));
    *fishingSpot = entirePond[rand() % sizeofpond];
}

void checkStop(){
    char inputStr[BUFFER];
    memset(inputStr, '\0', BUFFER);

    arduino.ReadData(inputStr, BUFFER);
    //printf("%s\n", inputStr);
    if(strstr(inputStr, "stop") !=  NULL){
        printf("stop detected\n");
        startWait();
    }
}

int main(){
    /*srand(time(NULL));
    struct location temp;
    readArray();
    move_to(&temp, rand());
    return 0;*/

    char message[BUFFER];
    struct location loc{0, 0};
//    struct location opens[10] {0, 0};
    srand(time(NULL));

    initialize();
    startWait();

    //TODO: get csv file ******************************
    //TODO: Read crv file *******************************

    while(1){
        //move_to(&loc, rand());
        /*if(userTest){
            sprintf(message, "servo1.up\n");
            arduino.WriteData(message, strlen(message));
            cout<<"x: ";
            cin >>loc.xAxis;
            cout<<"y: ";
            cin>>loc.yAxis;
            Sleep(1000);
            //loc.xAxis = -100;
            //loc.yAxis = -100;
        }
        else{*/
            move_to(&loc, rand());
            //calculate which fish to grab and through it into loc
        //}

        sprintf(message, "servo1.up\n");
        arduino.WriteData(message, strlen(message));
        Sleep(500);
        checkStop();
        moveBot(stepper1, loc.xAxis);
        checkStop();
        Sleep(500);
        //Sleep(1000);
        moveBot(stepper2, loc.yAxis);
        Sleep(500);
        checkStop();
        sprintf(message, "servo2.ready\n");
        arduino.WriteData(message, strlen(message));
        checkStop();
        while(!waitFish());
        fish();
        Sleep(1000);
        checkStop();
        dropoff();
        Sleep(500);
        checkStop();
    }
    arduino.~Serial();
    return 0;
}
