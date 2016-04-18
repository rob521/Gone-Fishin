#include<Wire.h>
#include<Adafruit_MotorShield.h>
#include"utility/Adafruit_MS_PWMServoDriver.h"

const int MODE = DOUBLE;
const char delimiter = '.';
const long stepper1OverSteps = 10000, stepper2OverSteps = 10000; //when overtravel is triggered steppers are at this many steps
const char stepper1fwd = FORWARD, stepper2fwd = FORWARD, stepper1back = BACKWARD, stepper2back = BACKWARD;

//pins
int stepper1Home = 1, stepper2Home = 2, stepper1Over = 3, stepper2Over = 4;

String inputString;
bool stringComplete;

long stepper1Pos, stepper2Pos;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *stepper1 = AFMS.getStepper(200,2),
                      *stepper2 = AFMS.getStepper(200,1);

void setup() {
  Serial.begin(9600);

  AFMS.begin();
  stepper1->setSpeed(1000);
  stepper2->setSpeed(1000);
  
  inputString.reserve(200); //I do not need to reserve this much space

  stepperHome();
  serialClean();
}

void loop() {
  // put your main code here, to run repeatedly:

}


/***********************
* Make sure steps are in range
 **********************/
void myStep(String motor, long pos){
  //Stepper1
  if(motor == "stepper1"){
    while(stepper1Pos != pos){
      if(stepper1Pos < pos){
        stepper1->step(1,stepper1fwd, MODE);
        stepper1Pos++;
      }
      else if(stepper1Pos > pos){
        stepper1->step(1, stepper1back, MODE);
        stepper1--;
      }
      
      if(digitalRead(stepper1Home) == HIGH) stepper1Pos = 0;
      else if(digitalRead(stepper1Over) == HIGH) stepper1Pos = stepper1OverSteps;
    }
  }

  //stepper2
  else if(motor == "stepper2"){
    while(stepper2Pos != pos){
      if(stepper2Pos < pos){
        stepper2->step(1,stepper2fwd, MODE);
        stepper1Pos++;
      }
      else if(stepper2Pos > pos){
        stepper2->step(1, stepper2back, MODE);
        stepper2--;
      }
      
      if(digitalRead(stepper2Home) == HIGH) stepper2Pos = 0;
      else if(digitalRead(stepper2Over) == HIGH) stepper2Pos = stepper1OverSteps;
    }
  }
}

void stepperHome(){
  if(digitalRead(stepper1Home) == HIGH) stepper1->step(100, stepper1fwd, MODE);
  if(digitalRead(stepper2Home) == HIGH) stepper2->step(100, stepper2fwd, MODE);

  while(digitalRead(stepper1Home) == LOW)
    stepper1->step(1, stepper1back, MODE);
  while(digitalRead(stepper2Home) == LOW)
    stepper2->step(1, stepper2back, MODE);

  stepper1Pos = 0;
  stepper2Pos = 0;
}

void inputParse(String *command, String *amount){
  *command = "";
  *amount = "";

  int delimPos = inputString.indexOf(delimiter);
  int inputLen = inputString.length();
  
  *command = inputString.substring(delimPos);
  *amount = inputString.substring(delimPos, inputLen);
}

void serialClean(){
  inputString = "";
  stringComplete = false;
}

void serialEvent(){
  char inChar;
  while(Serial.available()){
    inChar = (char)Serial.read();
    inputString += inChar;
    if(inChar == (char)13){
      stringComplete = true;
    }
  }
}

