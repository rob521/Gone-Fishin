#include<Servo.h>

bool pos = 0;
int i = 0;

Servo servo1, servo2;


void setup() {
  servo1.attach(10);
  servo2.attach(9);
}

void loop() {
  if(i < 5){
    servo1.write(180 * pos);
    servo2.write(150 * pos);
    pos = !pos;
    i++;
    delay(2000);
  }
  /*servo1.write(180 * pos);
  servo2.write(10 * pos);
  pos = !pos;
  delay(2000);*/
}
