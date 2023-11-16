//#include "Arduino.h"
// ** add header files to libraries folder in arduino ide
// header file containing all funtion definitions: Sensewall, default_mode, gradientControl, Reverse

//10/4/2023
// created by Matthew Alaniz, 
//sourced from the QuicRun_WP_880_Dual_Brushed_Test_Code_V1 
//header file containing funtion definitions

#ifndef _BTP_H   
#define _BTP_H    

#include "constants.h"

void default_mode(int distanceBack){
    if(distanceBack > 10){
      reverse(reverse_speed);
    }
}

double getdistance(int triggerPin,int echoPin){
    digitalWrite (triggerPin, HIGH);        //Turn on send trigger signal to back ultrasonic sensor
    delay(50);                            //Delay for 0.05 seconds to 50 ms
    digitalWrite(triggerPin, LOW);          //Turn off send trigger signal to back ultrasonic sensor
    duration = pulseIn(echoPin, HIGH);  //Set duration equal to the pulse reading from the echo of the back ultrasonic sensor
    double d = (duration/2)/29.1;         //Calculate the distance into cm
    return d;
}

void gradientControl(int target_speed)
{
  if(curr_speed > target_speed)
  {
    for(int i = curr_speed;i >= target_speed;i--)
    {
      servoLeft.write(i);                  //Set left side motors postion to 82
      servoRight.write(i);                 //Set left side motors postion to 82
      delay(100);
    }
    curr_speed = target_speed;
  }
  else if(curr_speed < target_speed)
  {
    for(int i = curr_speed;i <= target_speed;i++)
    {
      servoLeft.write(i);                  //Set left side motors postion to 82
      servoRight.write(i);                 //Set left side motors postion to 82
      delay(100);
    }
    curr_speed = target_speed;
  }
  else
  {
    curr_speed = target_speed;
  }
}

void reverse(int speed)
{
  if(is_reverse == false){
    servoLeft.write(break_point);                  //Set left side motors postion to 82
    servoRight.write(break_point);                 //Set left side motors postion to 82
    gradientControl(speed);
    is_reverse = true;
  }
}

#endif
