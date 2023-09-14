//Goal: have it crawl along the rail and respond to ultrasonic sensor
#include <Servo.h>
const int pwm1 = 6; // pin 6 as pwm
const int trigPin = 9;
const int echoPin = 10;

Servo servo;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  servo.attach(6, 1000, 2000);
  pinMode(pwm1, OUTPUT); //declare pin pwm as OUTPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT)
  servo.write(95);
  delay(5000);
}

void loop() {
  servo.write(100);
  delay(2000);
  servo.write(80);
  delay(2000);
}
