//Goal: Complete one half cycle using graident descent
//Created By: Alexander Ov
//Created On: 9/28/23
//Version 0.9
//Updates: 
/*
//General Notes:
So in the code, you need to make a function that overrides brake mode as follows:
Send 80-90 degrees to the pod to make it brake. Do not send 0. If i remember correctly, these ESCs are kind of 
violent, and 0 or 180 will send the pod flying in one direction or another.
From this point, use the gradient descent function to gradually increase the value back to your midpoint. 
For example, if your calibrated center point was at 92, and your brake degree is at 80, then your function would 
send the following information to the ESC with a 50ms delay in between: 80 -> 81 -> 82 -> 83 -> 84 -> ... -> 92
Once the control point is set back to 92, or the midpoint, then use the gradient descent back in the other direction 
to send the pod backwards. 92 -> 91 -> 90 -> .. you get the point
-Chris Lai
"Forward" and "Backward" is arbitarty for now

91 good midpoint for left side (false)
As midpoint decrease power to motor increase for forward

To make the code work the ESCs need to be in option 2 run mode
//Progress Report Notes
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
int midpoint = 90;
int curr_speed = 90;
int break_point = 85;
int forward_speed = 98;
int reverse_speed = 76;

//Function Decleration
void senseWall();                         //Function to determine which ultrasonic sensor to use and switch distance
void gradientControl(int target_speed);
void reverse(int speed);

//Setup Function
void setup() {
  Serial.println("Begin Setup");
  Serial.begin(9600);                     //Set baudrate to 9600

  servoLeft.attach(9, 1000, 2000);        //Attaches the motor driver on pin 9 to the servoLeft object (pin, min pulse width, max pulse width in microseconds)

  servoRight.attach(10, 1000, 2000);      //Attaches the motor driver on pin 10 to the servoRight object
  pinMode(led, OUTPUT);                   //LED pin declerartion as OUTPUT
  pinMode(led2, OUTPUT);                   //LED pin declerartion as OUTPUT
  pinMode(trigFront, OUTPUT);             //trigFront pin declerartion as OUTPUT
  pinMode(echoFront, INPUT);              //echoFront pin declerartion as OUTPUT
  pinMode(power, OUTPUT);                 //power pin declerartion as OUTPUT

  digitalWrite(led,HIGH);                 //Turn on LED for set up
  digitalWrite(led2,HIGH);                //Turn on LED for reverse movement state
  digitalWrite(power, HIGH);              //Turn on power for set up

  servoLeft.write(midpoint);                    //Set left side motors postion to 90
  servoRight.write(midpoint);                   //Set right side motors postion to 90
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
    gradientControl(forward_speed);
    Serial.println("Moving forward");     //Print to consle motors are moving forward
  }
  else if(direction == -1)                //Check if direction is set as backwards
  {
    digitalWrite(led2,HIGH);               //Turn on LED for reverse movement state
    reverse(reverse_speed);
    Serial.println("Moving reverse");     //Print to consle motors are moving backwards
  }
  else{                                   //Check if direction not forward or backward
    curr_speed = 90;
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
    Serial.println("Sense Reverse Direction");
    delay(2000);                          //Wait 3 seconds
    direction = 2;                        //Set direction to none to trigger a stop
  }
  else{                                   //Check if direction is set as forward
    Serial.println("Sense NO Direction");
    delay(10000);                         //Simulate 20 seconds of arm pulling out battery
    direction = -1;                       //Set pod to move backwards
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
  servoLeft.write(break_point);                  //Set left side motors postion to 82
  servoRight.write(break_point);                 //Set left side motors postion to 82
  gradientControl(midpoint);
  delay(100);
  gradientControl(speed);
}