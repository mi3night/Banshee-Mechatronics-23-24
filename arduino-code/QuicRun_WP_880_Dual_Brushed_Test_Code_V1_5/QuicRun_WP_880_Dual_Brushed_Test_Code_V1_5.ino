//Goal: Full functionallity using both ultrasonic sensor and get information from raspberry pi to tell Arduino Nano when to move
//Created By: Alexander Ov
//Created On: 11/1/23
//Version 1.5
/*
//
TODO:

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
#include "BTP.h" 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//String py_input = "";
//Setup Function
const int reset_pin = 13;

void setup() {
  //Serial communication startup
  Serial.begin(9600);                           //Set baudrate to 9600

  //Servo attachment
  servoLeft.attach(left_servos, 1000, 2000);    //Attaches the motor driver on pin 9 to the servoLeft object (pin, min pulse width, max pulse width in microseconds)
  servoRight.attach(right_servos, 1000, 2000);  //Attaches the motor driver on pin 10 to the servoRight object (pin, min pulse width, max pulse width in microseconds)

  //Display setup 
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

  //Pin IO Setup
  pinMode(blue_led, OUTPUT);                    //LED pin declerartion as OUTPUT
  pinMode(green_led, OUTPUT);                   //LED pin declerartion as OUTPUT
  pinMode(trigFront, OUTPUT);                   //trigFront pin declerartion as OUTPUT
  pinMode(echoFront, INPUT);                    //echoFront pin declerartion as OUTPUT
  pinMode(trigBack, OUTPUT);                    //trigFront pin declerartion as OUTPUT
  pinMode(echoBack, INPUT);                     //echoFront pin declerartion as OUTPUT
  pinMode(power, OUTPUT);                       //power pin declerartion as OUTPUT
  pinMode(reset_pin, INPUT);
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
  direction = 'S';                              //Set direction var to forward (BVM)
  //default_mode(distanceBack);                   //Reset Pod to the start point (GCS)
}

//Main Looping Fucntion
void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("BTP Armed");
  display.display();

  while(digitalRead(reset_pin) == LOW);  
  distance = getdistance(trigFront,echoFront);
  distanceBack = getdistance(trigBack,echoBack);

  display.clear()
  display.setCursor(0,0);
  display.print("Distance to BVM");
  display.setCursor(16, 0);
  display.print(distance);
  display.setCursor(0,1);
  display.print("Distance to GCS");
  display.setCursor(16,1);
  display.print(distanceBack);
  display.display();

  delay(50);

  if (Serial.available()>0) {
    char mode = Serial.read();

    switch(mode) {
      case 'b':                       //Mode 1: Pod moving to the right
        direction = 'B';        
        break;
      case 'g':                       //Mode 2: Pod moving to the left
        direction = 'G';
        break;
      default:
        break;
    }
  }

  if(direction == 'G')                          //Check if direction is set as forward
  {
    digitalWrite(blue_led,HIGH);                //Turn on LED for forward movement state
    digitalWrite(green_led,LOW);                //Turn on LED for reverse movement state
    gradientControl(forward_speed);
    if(distanceBack < 10)                       //Check if the distance is less than or equal to 10 cm  from the front
    {
      direction = 'S';                          //Set direction to none to trigger a stop
      Serial.write('1');
    }
  }
  else if(direction == 'B')                     //Check if direction is set as backwards (GCS)
  {      
    digitalWrite(blue_led,LOW);                 //Turn on LED for reverse movement state
    digitalWrite(green_led,HIGH);               //Turn on LED for reverse movement state
    reverse(reverse_speed);
    if(distance < 10)                           //Check if the distance is less than or equal to 10 cm from the back
    {
      is_reverse = false;
      direction = 'S';                          //Set direction to none to trigger a stop
      Serial.write('1');
    }
  }
  else if(direction == 'S'){                                         //Check if direction not forward or backward
    digitalWrite(blue_led,LOW);                 //Turn on LED for reverse movement state
    digitalWrite(green_led,LOW);               //Turn on LED for reverse movement state
    servoLeft.write(midpoint);                    //Set left side motors postion to 90
    servoRight.write(midpoint);                   //Set right side motors postion to 90
  }
}
