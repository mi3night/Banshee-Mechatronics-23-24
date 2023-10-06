//#include "Arduino.h"
// ** add header files to libraries folder in arduino ide
// header file containing all funtion definitions: Sensewall, default_mode, gradientControl, Reverse

//10/4/2023
// created by Matthew Alaniz, 
//sourced from the QuicRun_WP_880_Dual_Brushed_Test_Code_V1 
//header file containing funtion definitions

#ifndef _BTP_H   
#define _BTP_H    

#include <constants.h> 

void default_mode(int distanceBack){
    if(distanceBack > 10){
      reverse(reverse_speed);
    }
}

void senseWall()
{
  if(direction == 1)                      //Check if direction is set as forward
  {
    Serial.println("Sense fwd Direction");
    digitalWrite (trigFront, HIGH);       //Turn on send trigger signal to front ultrasonic sensor
    delay(50);                            //Delay for 0.05 seconds to 50 ms
    digitalWrite(trigFront, LOW);         //Turn off send trigger signal to front ultrasonic sensor
    duration = pulseIn(echoFront, HIGH);  //Set duration equal to the pulse reading from the echo of the front ultrasonic sensor
    distance = (duration/2)/29.1;         //Calculate the distance into cm
    if(distance < 10)                    //Check if the distance is less than or equal to 10 cm  from the front
    {
      direction = 0;                      //Set direction to none to trigger a stop
    }
  }
  else if(direction == -1)                //Check if direction is set as reverse
  {
    Serial.println("Sense bwd Direction");
    digitalWrite (trigBack, HIGH);        //Turn on send trigger signal to back ultrasonic sensor
    delay(50);                            //Delay for 0.05 seconds to 50 ms
    digitalWrite(trigBack, LOW);          //Turn off send trigger signal to back ultrasonic sensor
    duration = pulseIn(echoBack, HIGH);  //Set duration equal to the pulse reading from the echo of the back ultrasonic sensor
    Serial.println(duration);
    distance = (duration/2)/29.1;         //Calculate the distance into cm
    Serial.println(distance);
    if(distance < 15)                    //Check if the distance is less than or equal to 10 cm from the back
    {
      is_reverse = false;
      direction = 0;                      //Set direction to none to trigger a stop
    }
  }
  else{                                   //Check if direction is set as forward
    //Test without serial in
    // digitalWrite (trigBack, HIGH);        //Turn on send trigger signal to back ultrasonic sensor
    // delay(50);                            //Delay for 0.05 seconds to 50 ms
    // digitalWrite(trigBack, LOW);          //Turn off send trigger signal to back ultrasonic sensor
    // durationBack = pulseIn(echoBack, HIGH);  //Set duration equal to the pulse reading from the echo of the back ultrasonic sensor
    // Serial.println(duration);
    // distanceBack = (durationBack/2)/29.1;         //Calculate the distance into cm
    // Serial.println(distance);
    // if(distanceBack < 15)                    //Check if the distance is less than or equal to 10 cm from the back
    // {
    //   direction = 0;                      //Set direction to none to trigger a stop
    // }
    // else{
    //   delay(5000);
    //   direction = -1;
    // }

    //Shawn Serial input
    while(Serial.available())
    {
      String incomingData = Serial.readStringUntil('\n');
      py_input = incomingData.toInt();
    }
    //Original serial in
    if(py_input == 1) //For GCS
    {
      direction = -1;
    }
    else if(py_input == 2) //For BVM
    {
      direction = 1;
    }
    else{
      direction = 0;
    }
  }
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
    gradientControl(midpoint);
    delay(100);
    gradientControl(speed);
    is_reverse = true;
  }
}

#endif
