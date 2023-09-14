#include <Servo.h>
const int pwm1 = 6; // pin 6 as pwm
Servo servo;
int potValue = 0;

void setup() {
  servo.attach(6,1000,2000);// (pin, min pulse width, max pulse width in microseconds) 
  Serial.begin(9600);
  pinMode(pwm1,OUTPUT); //declare pin pwm as OUTPUT
}

void loop() {
  potValue = analogRead(A2);
  potValue = map(potValue,0,1023,0,180);
  servo.write(potValue);
}
