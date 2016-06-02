#include<Wire.h>
#include<Adafruit_MotorShield.h>
#include<Servo.h>
#include"arduino_main.h"
#include"utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *stepper1 = AFMS.getStepper(200,2),
                      *stepper2 = AFMS.getStepper(200,1);
void setup() {
  Serial.begin(9600);

  AFMS.begin();
  stepper1->setSpeed(1000);
  stepper2->setSpeed(1000);

  servo1.attach(10);
  servo2.attach(9);
  servo1.write(30);
  servo1Pos = 30;
  servo2.write(30);
  servo2Pos = 30;

  pinMode(start, INPUT);

  command.reserve(SERIAL_BUFFER);
  amount.reserve(SERIAL_BUFFER);
//  stepperHome();
  serialClean();

  /********************************
   * testing portion
   *******************************/
   stepper1Pos = 0;
   stepper2Pos = 0;
   startsig = true;
   //Serial.println("start");
}

void loop() {
  unsigned long runTime;
  int commandInt, amountInt;
  //checking if game is standby
  if(digitalRead(start) == HIGH && !startsig){
    Serial.println("stop");
    delay(250);
    startsig = true;
  }
  else if (digitalRead(start) == LOW && startsig){
    delay(250);
    Serial.println("start");
    startsig = false;
  }

//  while(digitalRead(start) == LOW){
if(stringComplete){
    /*Serial.print(command);
    Serial.print('.');
    Serial.print(amount);*/
    ID(&commandInt, &amountInt);

    switch(commandInt){
      case stepper1_:
        switch(amountInt){
          case is_:
            Serial.print(stepper1Pos);
            Serial.print("\n");
            break;
          case home_:
            //NEED TO ADD A HOME**********************
          default:
            myStep(commandInt, amountInt);
        }
        break;
        
      case stepper2_:
        switch(amountInt){
          case is_:
            Serial.print(stepper2Pos);
            Serial.print("\n");
            break;
          case home_:
            //NEED TO ADD A HOME************************
            break;
          default:
            myStep(commandInt, amountInt);
        }
        break;
        
      case servo1_:
      runTime = millis();
        switch(amountInt){
          case up_:
            myServo(commandInt, servo1Up, amountInt);
            runTime = (millis() - runTime) / 1000;
            Serial.print(runTime);
            Serial.println(" seconds");
            break;
          case down_:
            myServo(commandInt, servo1Down, amountInt);
            runTime = (millis() - runTime) / 1000;
            Serial.print(runTime);
            Serial.println(" seconds");
        }
        break;
        
      case servo2_:
      runTime = millis();
        switch(amountInt){
          case up_:
            myServo(commandInt, servo2Up, amountInt);
            runTime = (millis() - runTime) / 1000;
            Serial.print(runTime);
            Serial.println(" seconds");
            break;
          case down_:
            myServo(commandInt, servo2Down, amountInt);
            runTime = (millis() - runTime) / 1000;
            Serial.print(runTime);
            Serial.println(" seconds");
            break;
          case ready_:
            myServo(commandInt, servo2Ready, amountInt);
            runTime = (millis() - runTime) / 1000;
            //Serial.print(runTime);
            //Serial.println(" seconds");
            break;
          case shake_:
            shakeIt();
        }
        break;
        
      defualt:
        Serial.println("error");
    }
    serialClean();
  }

}

void shakeIt(){
  for(int i = 0; i < shakeNum; i++){
    servo2.write(shakeDown);
    delay(shakeDelay);
    servo2.write(shakeUp);
    delay(shakeDelay);
  }
}

/***********************
* Make sure steps are in range
 **********************/
void myServo(int motor, int pos, int choice){
  if(motor == servo1_){
    while(servo1Pos < pos){
      servo1Pos++;
      servo1.write(servo1Pos);
      delay(servoDelay);
    }
    while(servo1Pos > pos){
      servo1.write(servo1Pos);
      delay(servoDelay);
      servo1Pos--;
    }
  }
  else if(motor == servo2_){
    for(servo2Pos; servo2Pos < pos; servo2Pos++){
      servo2.write(servo2Pos);
      if (choice == ready_)
            delay(servoDelayDown);//MODIFIED:***************************************
      else
            delay(servoDelay);
    }
    for(servo2Pos; servo2Pos > pos; servo2Pos--){
      servo2.write(servo2Pos);
      delay(servoDelay);
    }
  }
}
void myStep(int motor, long pos){
  //Stepper1
  if(motor == stepper1_){
    while(stepper1Pos != pos){
      if(stepper1Pos < pos){
        stepper1->step(1,stepper1fwd, MODE);
        stepper1Pos++;
      }
      else if(stepper1Pos > pos){
        stepper1->step(1, stepper1back, MODE);
        stepper1Pos--;
      }
      
      //if(digitalRead(stepper1Home) == HIGH) stepper1Pos = 0; **************************************************
      //else if(digitalRead(stepper1Over) == HIGH) stepper1Pos = stepper1OverSteps; ******************************************
    }
  }

  //stepper2
  else if(motor == stepper2_){
    while(stepper2Pos != pos){
      if(stepper2Pos < pos){
        stepper2->step(1,stepper2fwd, MODE);
        stepper2Pos++;
      }
      else if(stepper2Pos > pos){
        stepper2->step(1, stepper2back, MODE);
        stepper2Pos--;
      }
      
      //if(digitalRead(stepper2Home) == HIGH) stepper2Pos = 0;************************************************
      //else if(digitalRead(stepper2Over) == HIGH) stepper2Pos = stepper1OverSteps;********************************************
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

void ID(int *commandInt, int *amountInt){
  if (command == "stepper1")
    *commandInt = stepper1_;
  else if (command == "stepper2")
    *commandInt = stepper2_;
  else if (command == "servo1")
    *commandInt = servo1_;
  else if (command == "servo2")
    *commandInt = servo2_;

  if(amount == "up")
    *amountInt = up_;
  else if (amount == "down")
    *amountInt = down_;
  else if (amount == "home")
    *amountInt = home_;
  else if (amount == "is")
    *amountInt = is_;
  else if (amount == "ready")
    *amountInt = ready_;
  else if (amount == "shake")
    *amountInt = shake_;
  else
    *amountInt = amount.toInt();
}

void serialClean(){
  command = "";
  amount = "";
  stringComplete = false;
  inputType = false;
}

void serialEvent(){
  char inChar;
  while(Serial.available()){
    inChar = (char)Serial.read();
    if(inChar == (char)13 || inChar == '\n')
      stringComplete = true;
    else if (inChar == delimiter)
      inputType = true;
    else if(inputType)
      amount += inChar;
    else 
      command += inChar;
  }
}

