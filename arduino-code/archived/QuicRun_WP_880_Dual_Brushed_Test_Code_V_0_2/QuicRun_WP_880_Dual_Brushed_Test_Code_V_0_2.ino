//Goal: Code the motors to be control by a potentiometer
//Created By: Alexander Ov
//Created On: 9/13/23
//Version 0.2
//Updates: 
/*
//General Notes:
//Progress Report Notes

*/

//Libraries
#include <Servo.h>                        //Servo lib
const int pwm1 = 6;                       //pwm pin decleration

//Object Decleration
Servo servo;                              //Create servo object to control the Battery Transfer Pod's DC Motors

//Global Variable Decleration
int potValue = 0;                         //Potentiometer var used for motor movement

//Setup Function
void setup() {
  servo.attach(6,1000,2000);              //Attaches the motor driver on pin 6 to the servo object (pin, min pulse width, max pulse width in microseconds) 
  Serial.begin(9600);                     //Set baudrate to 9600
  pinMode(pwm1,OUTPUT);                   //Declare pin pwm1 as OUTPUT
}

//Main Looping Fucntion
void loop() {
  potValue = analogRead(A2);              //Set potValue equal to the analog value of the A2 pin
  potValue = map(potValue,0,1023,0,180);  //Map the potentiometer values to match the servo
  servo.write(potValue);                  //Print to consle setup the potentiometer value
}
