//Goal: Have Battery Transfer Pod crawl back and forth on the rail
//Updates pwm1 on pcb doesnt work so we will have to use pwm1 for "pwm2" and power for "dir"
#include <Servo.h> 
const int power = 5; //9 works
const int pwm1 = 10; // pin 6 as pwm
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
  pinMode(echoPin, INPUT);
  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);
  servo.write(95);
  delay(5000);
}

void loop() {
  if(power){
    servo.write(97);
    Serial.println("Forward");
    delay(2000);
    servo.write(90);
    delay(2000);
    servo.write(83);
    Serial.println("Backward");
    delay(2000);
    servo.write(90);
    digitalWrite(power,LOW);
  }
}