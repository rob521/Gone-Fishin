#include<Wire.h>
#include<Adafruit_MotorShield.h>
#include<Servo.h>
#include"utility/Adafruit_MS_PWMServoDriver.h"

const int MODE = DOUBLE;

String inputstring = "";
bool motorDirection = 1, stringComplete = false;
long steps = 0;

Servo servo1, servo2;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *stepper = AFMS.getStepper(200, 2);

void setup() {
  //setup for stepper
  Serial.begin(9600);
  AFMS.begin();
  stepper->setSpeed(1000);

  //setup for reading serial
  inputstring.reserve(200);

  servo1.attach(10);
  servo2.attach(9);

  //setup for interrupts
  pinMode(2, INPUT  );
  attachInterrupt(0, halt, CHANGE);

  Serial.println("start");
}

void loop(){
  if(stringComplete && motorDirection){
    stepper->step(steps, FORWARD, MODE);
    servoMove();
    inputstring = "";
    stringComplete = false;
    motorDirection = 0;
    steps = 0;
  }
  if(stringComplete && !motorDirection){
    stepper->step(steps, BACKWARD, MODE);
    servoMove();
    inputstring = "";
    stringComplete = false;
    motorDirection = 1;
    steps = 0;
  }
}

void servoMove(){
  if(inputstring == "up"){
    servo1.write(180);
    servo2.write(100);
  }
  else{
    servo1.write(0);
    servo2.write(0);
  }
}

void halt(){
  Serial.println("Interrupted");
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar >= '0' && inChar <= '9')
      steps = steps * 10 + atol(&inChar);
    else if(inChar == (char)13 || inChar == '\n') 
      stringComplete = true;
    else    
      inputstring += inChar;
  }
}

