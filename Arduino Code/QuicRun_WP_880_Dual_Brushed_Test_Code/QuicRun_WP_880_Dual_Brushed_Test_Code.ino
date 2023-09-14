#include <Servo.h>
const int pwm1 = 6; // pin 6 as pwm
Servo servo;
int pos = 0;

void setup() {
  servo.attach(6,1000,2000);
  Serial.begin(9600);
  pinMode(pwm1,OUTPUT); //declare pin pwm as OUTPUT
  servo.write(90);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}