#include<Wire.h>
#include<Adafruit_MotorShield.h>
#include"utility/Adafruit_MS_PWMServoDriver.h"

const int MODE = DOUBLE;

String inputstring = "";
bool motorDirection = 1, stringComplete = false;
long steps = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *stepper = AFMS.getStepper(200, 2);

void setup() {
  //setup for stepper
  Serial.begin(9600);
  AFMS.begin();
  stepper->setSpeed(1000);

  //setup for reading serial
  inputstring.reserve(200);

  //setup for interrupts
  pinMode(2, INPUT  );
  attachInterrupt(0, halt, CHANGE);

  Serial.println("start");
}

void loop(){
  if(stringComplete && motorDirection){
    stepper->step(steps, FORWARD, MODE);
    Serial.println(inputstring);
    inputstring = "";
    stringComplete = false;
    motorDirection = 0;
    steps = 0;
  }
  if(stringComplete && !motorDirection){
    stepper->step(steps, BACKWARD, MODE);
    Serial.println(inputstring);
    inputstring = "";
    stringComplete = false;
    motorDirection = 1;
    steps = 0;
  }
}

void halt(){
  /*limit = true;
  delay(2);
  limit = false;*/
  Serial.println("Interrupted");
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar >= '0' && inChar <= '9')
      steps = steps * 10 + atol(&inChar);
      
    // add it to the inputString:
    inputstring += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == (char)13) {
      stringComplete = true;
    }
  }
}

