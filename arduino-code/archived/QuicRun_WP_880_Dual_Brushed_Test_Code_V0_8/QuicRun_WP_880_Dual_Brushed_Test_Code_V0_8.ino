//Goal: Complete one half cycle
//Created By: Alexander Ov
//Created On: 9/25/23
//Version 0.8
//Updates: 
/*
//General Notes:
"Forward" and "Backward" is arbitarty for now
//Progress Report Notes
Make sure 5V rail is powered by arduino
ESC doesnt move in reverse
70 going reverse with out rail is prefect
now 70 doesnt work
possibly a low battery lipo problem (not a lipo problem)
*/

//Libraries
#include <Servo.h>                        //Servo lib

//Pin Declerations
const int power = 5;                      //vcc toggle pin decleration(9 also works)
const int trigFront = 3;                  //ultrasonic sensor trig pin decleration
const int echoFront = 4;                  //ultrasonic sensor echo pin decleration

const int trigBack = 7;                   //ultrasonic sensor trigBack pin decleration NOTE CHANGE PINS
const int echoBack = 8;                   //ultrasonic sensor echoBack pin decleration NOTE CHANGE PINS
const int led = 11;                       //led pin decleration
const int led2 = 2;                       //TEMP LED

//Object Decleration
Servo servoLeft;                          //Create servo object to control the Battery Transfer Pod's left side DC Motors 
Servo servoRight;                         //Create servo object to control the Battery Transfer Pod's right side DC Motors 

//Global Variable Decleration
long duration;                            //duration var used for ultrasonic sensor calculations
int distance;                             //distance var used for ultrasonic sensor calculations
int direction;                            //direction var used to determine which way the motors should spin

//Function Decleration
void senseWall();                         //Function to determine which ultrasonic sensor to use and switch distance

//Setup Function
void setup() {
  Serial.println("Begin Setup");
  Serial.begin(9600);                     //Set baudrate to 9600

  servoLeft.attach(9, 1000, 2000);        //Attaches the motor driver on pin 9 to the servoLeft object (pin, min pulse width, max pulse width in microseconds)
  pinMode(9, OUTPUT);                     //declare pin 9 as OUTPUT (not needed?)

  servoRight.attach(10, 1000, 2000);      //Attaches the motor driver on pin 10 to the servoRight object
  pinMode(10, OUTPUT);                    //declare pin 10 as OUTPUT (not needed?)
  pinMode(led, OUTPUT);                   //LED pin declerartion as OUTPUT
  pinMode(led2, OUTPUT);                   //LED pin declerartion as OUTPUT
  pinMode(trigFront, OUTPUT);             //trigFront pin declerartion as OUTPUT
  pinMode(echoFront, INPUT);              //echoFront pin declerartion as OUTPUT
  pinMode(power, OUTPUT);                 //power pin declerartion as OUTPUT

  digitalWrite(led,HIGH);                 //Turn on LED for set up
  digitalWrite(led2,HIGH);                //Turn on LED for reverse movement state
  digitalWrite(power, HIGH);              //Turn on power for set up

  servoLeft.write(90);                    //Set left side motors postion to 90
  servoRight.write(90);                   //Set right side motors postion to 90
  delay(5000);                            //Delay for 5 seconds
  digitalWrite(led,LOW);                  //Turn off LED for set up
  digitalWrite(led2,LOW);               //Turn on LED for reverse movement state
  Serial.println("Setup complete");       //Print to consle setup is complete
  direction = 1;                          //Set direction var to forward
}

//Main Looping Fucntion
void loop() {
  senseWall();                            //Call senseWall function
  Serial.println(direction);              //Print to consle which direction is going
  if(direction == 1)                      //Check if direction is set as forward
  {
    digitalWrite(led,HIGH);               //Turn on LED for forward movement state
    servoLeft.write(98);                  //Set left side motors postion to 98
    servoRight.write(98);                 //Set right side motors postion to 98
    Serial.println("Moving forward");     //Print to consle motors are moving forward
  }
  else if(direction == -1)                //Check if direction is set as backwards
  {
    digitalWrite(led2,HIGH);               //Turn on LED for reverse movement state
    servoLeft.write(70);                  //Set left side motors postion to 82
    servoRight.write(70);                 //Set right side motors postion to 82
    Serial.println("Moving reverse");     //Print to consle motors are moving backwards
  }
  else{                                   //Check if direction not forward or backward
    digitalWrite(led,LOW);                //Turn off LED for no movement state
    digitalWrite(led2,LOW);               //Turn on LED for reverse movement state
    servoLeft.write(90);                  //Set left side motors postion to 82
    servoRight.write(90);                 //Set left side motors postion to 82
    Serial.println("Pod reset");          //Print to consle motors are reset
  }
}

//Sense Wall Function
void senseWall()
{
  if(direction == 1)                      //Check if direction is set as forward
  {
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
    // digitalWrite (trigBack, HIGH);        //Turn on send trigger signal to back ultrasonic sensor
    // delay(50);                            //Delay for 0.05 seconds to 50 ms
    // digitalWrite(trigBack, LOW);          //Turn off send trigger signal to back ultrasonic sensor
    // duration = pulseIn(echoFront, HIGH);  //Set duration equal to the pulse reading from the echo of the back ultrasonic sensor
    // distance = (duration/2)/29.1;         //Calculate the distance into cm
    delay(7000);                          //Wait 3 seconds
    direction = 0;                        //Set direction to none to trigger a stop
    // if(distance <= 10)                    //Check if the distance is less than or equal to 10 cm from the back
    // {
    //   direction = 0;                      //Set direction to none to trigger a stop
    // }
  }
  else{                                   //Check if direction is set as forward
    delay(10000);                         //Simulate 20 seconds of arm pulling out battery
    direction = -1;                       //Set pod to move backwards
  }
}
