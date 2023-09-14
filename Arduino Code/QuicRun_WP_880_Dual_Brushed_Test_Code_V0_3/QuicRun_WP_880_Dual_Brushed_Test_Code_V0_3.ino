#include <Servo.h>
const int pwm1 = 6; // pin 6 as pwm
Servo servo;

void motorSpin(long pos){
  servo.attach(pos);
}

void setup() {
  servo.attach(6,1000,2000);
  Serial.begin(9600);
  pinMode(pwm1,OUTPUT); //declare pin pwm as OUTPUT
  servo.write(95);
  delay(5000);
}

void loop() {
    
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   servo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }
  for(int i = 90; i <=120;i+=1){
    servo.write(i);
    delay(1000);
  }
}
