//Goal: Full functionallity using both ultrasonic sensor and get information from raspberry pi to tell Arduino Nano when to move
//Created By: Alexander Ov
//Created On: 10/02/23
//Version 1.0
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

GCS Request Battery -> BTP RPI sends signal to arm for extraction -> BTP RPI sends signal to the BTP Arduino to move to BVM -> BTP moves to BVM -> BVM
Updates:
V1.0 Updates
10/04/23
Modified by Jin
Update: 
1. Added a function that resets the pod to the start point (GCS).
2. Organized the comment of each line.
3. Deleted the unnecessary Print statement. 
4. Turned the variable "Direction" to char so we can easily identify the pod's direction.

Modified by Matthew
took out all the parameters and funtions and placed them into header files. 

10/04/23
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
#include <BTP.h> 




//Setup Function
void setup() {
  //Serial communication startup
  Serial.begin(9600);                           //Set baudrate to 9600

  //Servo attachment
  servoLeft.attach(left_servos, 1000, 2000);    //Attaches the motor driver on pin 9 to the servoLeft object (pin, min pulse width, max pulse width in microseconds)
  servoRight.attach(right_servos, 1000, 2000);  //Attaches the motor driver on pin 10 to the servoRight object (pin, min pulse width, max pulse width in microseconds)

  //Pin IO Setup
  pinMode(blue_led, OUTPUT);                    //LED pin declerartion as OUTPUT
  pinMode(green_led, OUTPUT);                   //LED pin declerartion as OUTPUT
  pinMode(trigFront, OUTPUT);                   //trigFront pin declerartion as OUTPUT
  pinMode(echoFront, INPUT);                    //echoFront pin declerartion as OUTPUT
  pinMode(trigBack, OUTPUT);                    //trigFront pin declerartion as OUTPUT
  pinMode(echoBack, INPUT);                     //echoFront pin declerartion as OUTPUT
  pinMode(power, OUTPUT);                       //power pin declerartion as OUTPUT

  //LED start up
  digitalWrite(blue_led,HIGH);                  //Turn on LED for set up
  digitalWrite(green_led,HIGH);                 //Turn on LED for reverse movement state
  digitalWrite(power, HIGH);                    //Turn on power for set up

  //ESC safety start up
  servoLeft.write(midpoint);                    //Set left side motors postion to 90
  servoRight.write(midpoint);                   //Set right side motors postion to 90
  delay(5000);                                  //Delay for 5 seconds
  digitalWrite(blue_led,LOW);                   //Turn off LED for set up
  digitalWrite(green_led,LOW);                  //Turn on LED for reverse movement state
  direction = 'B';                              //Set direction var to forward (BVM)
  default_mode(distanceBack);                   //Reset Pod to the start point (GCS)
}


//Main Looping Fucntion
void loop() {
  senseWall();                                  //Call senseWall function
  if(direction == 'B')                          //Check if direction is set as forward
  {
    digitalWrite(blue_led,HIGH);                //Turn on LED for forward movement state
    digitalWrite(green_led,LOW);                //Turn on LED for reverse movement state
    gradientControl(forward_speed);
  }
  else if(direction == 'G')                     //Check if direction is set as backwards (GCS)
  {
    digitalWrite(blue_led,LOW);                 //Turn on LED for reverse movement state
    digitalWrite(green_led,HIGH);               //Turn on LED for reverse movement state
    reverse(reverse_speed);
  }
  else{                                         //Check if direction not forward or backward
    digitalWrite(blue_led,LOW);                 //Turn off LED for no movement state
    digitalWrite(green_led,LOW);                //Turn on LED for reverse movement state
    gradientControl(midpoint);
  }
}
