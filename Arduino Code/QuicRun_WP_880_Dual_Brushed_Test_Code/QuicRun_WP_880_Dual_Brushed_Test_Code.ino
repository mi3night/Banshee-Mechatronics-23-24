//Goal: Test if we can drive the Battery Transfer Pod's motor 
//Created By: Alexander Ov
//Created On: 9/11/23
//Version 0.1
//Updates: 
/*
//Progress Report Notes

*/
//Libraries
#include <Servo.h>                          //Servo lib

//Pin Declerations
const int pwm1 = 6;                         // pin 6 as pwm

//Object Decleration
Servo servo;                                //Create servo object to control the Battery Transfer Pod's DC Motors

//Global Variable Decleration
int pos = 0;                                //Postion var used to determine postion of the motor

//Setup Function
void setup() {
  servo.attach(6,1000,2000);                //Attaches the motor driver on pin 6 to the servo object
  Serial.begin(9600);                       //Set baudrate to 9600
  pinMode(pwm1,OUTPUT);                     //declare pin pwm as OUTPUT
  servo.write(90);                          //Set motors postion to 90
}

//Main Looping function
void loop() {
  for (pos = 0; pos <= 180; pos += 1) {     // goes from 0 degrees to 180 degrees in steps of 1 degree
    servo.write(pos);                       // tell motor to go to position in variable 'pos'
    delay(15);                              // waits 15 ms for the servo to reach the position
  }
}