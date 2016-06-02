#ifndef _FISHINGCODE_
#define _FISHINGCODE_

const int SERIAL_BUFFER = 200;

const int MODE = DOUBLE,
          servoDelay = 5,//60, //15us per jump
          servoDelayDown = 60,
          servo1Up = 69,
          servo1Down = 10, //30
          servo2Up = 50,//30, 
          servo2Ready = 75,
          servo2Down = 100;

const int shakeNum = 10,
          shakeDelay = 60,
          shakeUp = 30,
          shakeDown = 50;

const char delimiter = '.';

const unsigned long stepper1OverSteps = 10000, //steps from home to over-travel sensor
                    stepper2OverSteps = 10000;
//determining stepper direction
const char stepper1fwd = FORWARD,
           stepper2fwd = FORWARD,
           stepper1back = BACKWARD,
           stepper2back = BACKWARD;

//setting pins
const int stepper1Home = 1,
          stepper2Home = 2,
          stepper1Over = 3,
          stepper2Over = 4,
          start = 2;//start pin

String command, amount;
bool stringComplete, inputType;

int stepper1Pos,
    stepper2Pos,
    servo1Pos,
    servo2Pos,
    startsig;

Servo servo1,
      servo2;

enum action{ stepper1_, stepper2_, servo1_, servo2_, up_, down_, ready_, shake_, is_, home_};

#endif // _FISHINGCODE_
