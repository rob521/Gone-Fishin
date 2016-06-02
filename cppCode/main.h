#ifndef _MAIN_
#define _MAIN_

#include<iostream>

using namespace std;

const bool userTest = false;

const char fileName[] = "Fish.csv";

const int BUFFER = 200,
          delaystabalize = 10000, // 50 sec delay
          csvOffset = 425;
const char port[] = "COM3",
           matlab[] = "matlabCode.exe";
Serial arduino((char *)port);

enum sendCommands{stepper1, stepper2, servo1, servo2};
enum recieveCommands{up, down, stop, start};

struct location{
    int xAxis, yAxis;
};

const struct location dropoffLoc{-200, 0};//*********************************************

//Functions
void moveBot(int, int);

#endif // _MAIN_
