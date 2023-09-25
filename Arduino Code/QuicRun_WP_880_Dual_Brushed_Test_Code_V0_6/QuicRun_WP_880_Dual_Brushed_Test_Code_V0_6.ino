//Goal: have it crawl along the rail and react to ultra sonic sensor
//Created By: Alexander Ov
//Created On: 9/18/23
//Updates: 
/*
9/18/23

//Progress Report Note
Pod wheels should be screwed on the flat side of the motor axel rather than the round side
otherwise there will be problems with it rolling
motors are inconistant causing trouble with moving on rail due to large friction (fixed by securing wheels better)
TODD: wensday take off wheels and level pod to see if it had melted out in the sun over the summer
*/
#include <Servo.h> 
const int power = 5; //9 works
const int trig = 3;
const int echo = 4;
const int led = 11;
Servo servoFront;
Servo servoBack;

long duration;
int distance;

void setup() {
  Serial.begin(9600);

  servoFront.attach(9, 1000, 2000);
  pinMode(9, OUTPUT); //declare pin pwm as OUTPUT

  servoBack.attach(10, 1000, 2000);
  pinMode(10, OUTPUT); //declare pin pwm as OUTPUT
  pinMode(led, OUTPUT); //LED pin declerartion
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(power, OUTPUT);

  digitalWrite(led,HIGH);
  digitalWrite(power, HIGH);
  servoFront.write(90);
  servoBack.write(90);
  delay(5000);
  digitalWrite(led,LOW);
  Serial.println("Setup complete");
}

void loop() {
  digitalWrite (trig, HIGH);
  delay(50);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2)/29.1;
  
  if(distance > 10){
    digitalWrite(led,HIGH);
    servoFront.write(98);
    servoBack.write(98);
  }
  else{
    digitalWrite(led,LOW);
    servoFront.write(90);
    servoBack.write(90);
    Serial.println("Servo reset");
  }
}