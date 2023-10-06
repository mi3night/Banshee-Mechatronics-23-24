//Goal: Use the set up the ESC so that it complies with the safety protocol 
//Created By: Alexander Ov
//Created On: 9/13/23
//Version 0.3
// 
/*
Updates:
Code now uses set up time in compliance with the ESC saftey features and is able to spin
//General Notes:
OBSOLETE CODE
//Progress Report Notes

*/

//Libraries
#include <Servo.h>                        //Servo lib
const int pwm1 = 6;                       //pwm pin decleration

//Object Decleration
Servo servo;                              //Create servo object to control the Battery Transfer Pod's DC Motors

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
  for(int i = 90; i <=100;i+=1)
  {
    servo.write(i);
    delay(1000);
  }
}
