//Goal: have it crawl along the rail
//Created By: Alexander Ov
//Created On: 9/15/23
//Updates: 
/*
9/15/23
change pins so that it matches the Arduino Nano ie: no more pwm1 = 9, pwm is directly mapped
added coded that spins motor forward, reset, backward
note: before starting the circuit check pin outputs to ensure that vcc = 5v, pin 9 and pin 10 
should output about 300 mV depending on current movement forward(350 mV), back, or reset
When free spinning
96 slow only one 
97 is a very very slow crawl and spin independntly
98 steady but fast
100 is way too fast
On track
96 wont move
97 wont move
98 decent speed still fast
going across the pod the motors need to have oppiste polarity otherwise they will spin opp directions

//Progress Report Note
team switched pod wiring so the each esc controll sides indvidually then swap polaritary(did work if both vcc are plugged in)

*/
#include <Servo.h> 
const int power = 5; //9 works
// const int pwm1 = 10; // pin 6 as pwm
// const int trigPin = 9;
// const int echoPin = 10;
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
  // pinMode(trigPin, OUTPUT);
  // pinMode(echoPin, INPUT);
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
  if(power){
    digitalWrite(11,HIGH);
    servoFront.write(98);
    servoBack.write(98);
    Serial.println("Backward");
    delay(3000);
    digitalWrite(led,LOW);
    servoFront.write(90);
    servoBack.write(90);
    Serial.println("Servo reset");
    delay(10000);
    digitalWrite(11,HIGH);
    servoFront.write(70);
    servoBack.write(70);
    Serial.println("Forward");
    delay(3000);
    digitalWrite(led,LOW);
    servoFront.write(90);
    servoBack.write(90);
    Serial.println("Servo reset");
    delay(10000);
    digitalWrite(power,LOW);
  }
}