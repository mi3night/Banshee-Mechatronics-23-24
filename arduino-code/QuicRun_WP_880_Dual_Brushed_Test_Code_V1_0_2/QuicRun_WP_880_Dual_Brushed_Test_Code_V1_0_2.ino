//Goal: Add reset pin control so raspberry pi can control when the arduino code can run
//Created By: Alexander Ov
//Created On: 10/11/23
//Version 1.0.2
/*
//
TODO:
remove updates from comments and make an update log txt file or something like that along with a resource doc and readme
Matt: maybe make header files to keep code cleaner
readd vcc control
clean upcode ie make comments uniform, remove debugging print statements, make naming conventions and formatting conistant
Jin: Make a default or reset pod function to always be at the "start postion" (the GCS side of the rail)start direction at 0 by default rather than 1 since the rpi should be controlling the pod movement
  make sure that pod starts at no movement and check if its in the right spot

Alex: change serial read from pi to be only positive ints only since the string to int conversion cannot process the "-" sign possibly switch to string input only?

GCS Request Battery Sends singal to BTP RPI -> BTP RPI sends signal to arm for extraction -> BTP RPI sends signal to the BTP Arduino to move to BVM -> BTP moves to BVM -> BVM Already in empty state 
-> BTP Arduino signals to BTP RPI its at BVM -> BTP RPI inputs battery into empty slot of BVM -> BVM roates to new battery -> BTP RPI Arm Cam looks for AR Mark -> BTP RPI ARM grabs battery
-> BTP RPI sends singal to BTP Arduino to go to GCS -> BTP moves to GCS -> BTP Arduino singals to BTP RPI its at GCS -> BTP RPI inserts battery into Drone -> rinse and repeat

Updates:
V1.0 Updates
10/04/23
Added serial output 's' to BTP RPI that it has stopped
Switch serial input if statements to be chars instead of postive ints only GCS = 'g' BVM = 'b'
Switch serial input if statements to be positive ints only GCS = 1 BVM = 2

10/03/23
Added python Serial input from Shawn's code testing
Commented out old test code
Added pin declerations for left and right esc and updated comments so it matches pcb
Updated led names to match PCB description
Added LED meanings
10/02/23
Added code to test out the pod movement without rasberry pi input
V0.9 Updates
10/02/23
Changed pinouts to match the PCB
//General Notes:
Possiblely need streaming.h but from light research it just changes how to take in IO streams of data "C++-style Output with Operator"

If the code doesnt compile on the first run Go to Tools -> Processor -> Use ATmega328P (Old Bootloader)
To make the code work the ESCs need to be in option 2 run mode

LED Meanings
Blue ON  Green ON  => Setup
Blue ON  Green OFF => Forward
Blue OFF Green ON  => Reverse
Blue OFF Green OFF => Stopped

FOR BTP ONLY
int midpoint = 90;
int curr_speed = 90;
int break_point = 85;
int forward_speed = 98;
int reverse_speed = 76;

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
*/
//Libraries
#include <Servo.h>                        //Servo lib

//Pin Declerations
const int power = 5;                      //vcc toggle pin decleration
const int left_servos = 6;                //left esc pin decleration
const int right_servos = 10;              //right esc pin decleration
const int trigFront = 3;                  //ultrasonic sensor trig pin decleration
const int echoFront = 4;                  //ultrasonic sensor echo pin decleration
const int trigBack = 7;                   //ultrasonic sensor trigBack pin decleration
const int echoBack = 8;                   //ultrasonic sensor echoBack pin decleration
const int blue_led = 11;                  //blue led pin decleration
const int green_led = 12;                 //green led pin decleration
const int reset_pin = 13;
//Object Decleration
Servo servoLeft;                          //Create servo object to control the Battery Transfer Pod's left side DC Motors 
Servo servoRight;                         //Create servo object to control the Battery Transfer Pod's right side DC Motors 

//Global Variable Decleration
long duration;                            //duration var used for ultrasonic sensor calculations
int distance;                             //distance var used for ultrasonic sensor calculations

long durationBack;                            //duration var used for ultrasonic sensor calculations
int distanceBack;                             //distance var used for ultrasonic sensor calculations
int direction;                            //direction var used to determine which way the motors should spin
int midpoint = 90;
int curr_speed = 90;
int break_point = 85;
int forward_speed = 99;
int reverse_speed = 76;
bool is_reverse = false;

String py_input;

//Function Decleration
void senseWall();                         //Function to determine which ultrasonic sensor to use and switch distance
void gradientControl(int target_speed);
void reverse(int speed);

//Setup Function
void setup() {
  //Serial communication startup
  Serial.begin(9600);                     //Set baudrate to 9600

  //Servo attachment
  servoLeft.attach(left_servos, 1000, 2000);        //Attaches the motor driver on pin 9 to the servoLeft object (pin, min pulse width, max pulse width in microseconds)
  servoRight.attach(right_servos, 1000, 2000);      //Attaches the motor driver on pin 10 to the servoRight object (pin, min pulse width, max pulse width in microseconds)

  //Pin IO Setup
  pinMode(blue_led, OUTPUT);                   //LED pin declerartion as OUTPUT
  pinMode(green_led, OUTPUT);                   //LED pin declerartion as OUTPUT
  pinMode(trigFront, OUTPUT);             //trigFront pin declerartion as OUTPUT
  pinMode(echoFront, INPUT);              //echoFront pin declerartion as OUTPUT
  pinMode(trigBack, OUTPUT);             //trigFront pin declerartion as OUTPUT
  pinMode(echoBack, INPUT);              //echoFront pin declerartion as OUTPUT
  pinMode(power, OUTPUT);                 //power pin declerartion as OUTPUT
  pinMode(reset_pin, INPUT);

  //LED start up
  digitalWrite(blue_led,HIGH);                 //Turn on LED for set up
  digitalWrite(green_led,HIGH);                //Turn on LED for reverse movement state
  digitalWrite(power, HIGH);                  //Turn on power for set up
  //ESC safety start up
  servoLeft.write(midpoint);                    //Set left side motors postion to 90
  servoRight.write(midpoint);                   //Set right side motors postion to 90
  delay(8000);                            //Delay for 5 seconds
  digitalWrite(blue_led,LOW);                  //Turn off LED for set up
  digitalWrite(green_led,LOW);               //Turn on LED for reverse movement state
  direction = 0;                          //Set direction var to forward
}

//Main Looping Fucntion
void loop() {
  while(digitalRead(reset_pin) ==  LOW);
  senseWall();                            //Call senseWall function
  //Serial.println(direction);              //Print to consle which direction is going
  if(direction == 1)                      //Check if direction is set as forward
  {
    digitalWrite(blue_led,HIGH);               //Turn on LED for forward movement state
    digitalWrite(green_led,LOW);               //Turn on LED for reverse movement state
    gradientControl(forward_speed);
  }
  else if(direction == -1)                //Check if direction is set as backwards
  {
    digitalWrite(blue_led,LOW);               //Turn on LED for reverse movement state
    digitalWrite(green_led,HIGH);               //Turn on LED for reverse movement state
    reverse(reverse_speed);
  }
  else{                                   //Check if direction not forward or backward
    digitalWrite(blue_led,LOW);                //Turn off LED for no movement state
    digitalWrite(green_led,LOW);               //Turn on LED for reverse movement state
    gradientControl(midpoint);
    //Serial.println("Pod reset");          //Print to consle motors are reset
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
    digitalWrite (trigBack, HIGH);        //Turn on send trigger signal to back ultrasonic sensor
    delay(50);                            //Delay for 0.05 seconds to 50 ms
    digitalWrite(trigBack, LOW);          //Turn off send trigger signal to back ultrasonic sensor
    duration = pulseIn(echoBack, HIGH);  //Set duration equal to the pulse reading from the echo of the back ultrasonic sensor
    distance = (duration/2)/29.1;         //Calculate the distance into cm
    if(distance < 15)                    //Check if the distance is less than or equal to 10 cm from the back
    {
      is_reverse = false;
      direction = 0;                      //Set direction to none to trigger a stop
    }
  }
  else{                                   //Check if direction is set as forward
    //Shawn Serial input
    while(Serial.available())
    {
      py_input = Serial.readStringUntil('\n');
      digitalWrite(blue_led,HIGH);                  //Turn off LED for set up
      digitalWrite(green_led,HIGH);               //Turn on LED for reverse movement state
      delay(50);
      digitalWrite(blue_led,LOW);                  //Turn off LED for set up
      digitalWrite(green_led,LOW);               //Turn on LED for reverse movement state
      delay(50);
      digitalWrite(blue_led,HIGH);                  //Turn off LED for set up
      digitalWrite(green_led,HIGH);               //Turn on LED for reverse movement state
      delay(50);
      digitalWrite(blue_led,LOW);                  //Turn off LED for set up
      digitalWrite(green_led,LOW);               //Turn on LED for reverse movement state
      delay(50);
      digitalWrite(blue_led,HIGH);                  //Turn off LED for set up
      digitalWrite(green_led,HIGH);               //Turn on LED for reverse movement state
      delay(50);
      digitalWrite(blue_led,LOW);                  //Turn off LED for set up
      digitalWrite(green_led,LOW);               //Turn on LED for reverse movement state
      Serial.print(py_input);
    }
    //Original serial in
    if(py_input == 'g') //For GCS
    {
      direction = 1;
    }
    else if(py_input == 'v') //For BVM
    {
      direction = -1;
    }
    else{
      direction = 0;
      Serial.write('s');
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