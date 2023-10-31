//#include "Arduino.h"
// ** add header files to libraries folder in arduino ide
//header file containin all the constants/varaibles used by the pod's main script

//Created by Matthew Alaniz, soruced from QuicRun_WP_880_Dual_Brushed_Test_Code_V1
//10/4/2023


#ifndef _CONSTANTS_H   
#define _CONSTANTS_H 
#include <Servo.h>                        //Servo lib
//Pin Declerations
const int power = 5;                      //vcc toggle pin decleration
const int left_servos = 6;                //left esc pin decleration
const int right_servos = 10;              //right esc pin decleration
const int trigFront = 7;                  //ultrasonic sensor trig pin decleration
const int echoFront = 8;                  //ultrasonic sensor echo pin decleration
const int trigBack = 3;                   //ultrasonic sensor trigBack pin decleration
const int echoBack = 4;                   //ultrasonic sensor echoBack pin decleration
const int blue_led = 11;                  //blue led pin decleration
const int green_led = 12;                 //green led pin decleration

//Object Decleration
Servo servoLeft;                          //Create servo object to control the Battery Transfer Pod's left side DC Motors 
Servo servoRight;                         //Create servo object to control the Battery Transfer Pod's right side DC Motors 

//Global Variable Decleration
long duration;                            //duration var used for ultrasonic sensor calculations
int distance;  
long durationBack;                            //duration var used for ultrasonic sensor calculations
int distanceBack;                             //distance var used for ultrasonic sensor calculations
char direction;                            //direction var used to determine which way the motors should spin
int midpoint = 91;
int curr_speed = 90;
int break_point = 85;
int forward_speed = 99;
int reverse_speed = 77;
bool is_reverse = false;

int py_input;
//Function Decleration
void senseWall();                         //Function to determine which ultrasonic sensor to use and switch distance
void gradientControl(int target_speed);
void reverse(int speed);
void default_mode(int distanceBack);
int getdistance(int triggerPin,int echoPin);
#endif
