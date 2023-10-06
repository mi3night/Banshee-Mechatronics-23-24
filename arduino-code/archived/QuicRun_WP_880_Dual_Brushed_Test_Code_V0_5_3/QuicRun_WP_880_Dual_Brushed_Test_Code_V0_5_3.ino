//Goal: Test theory that the reset point is not 90
//Created By: Alexander Ov
//Created On: 9/27/23
//Updates: 
/*
Tested valules from 80 to 96 the reverse still wont work

//Progress Report Note
team switched pod wiring so the each esc controll sides indvidually then swap polaritary(did work if both vcc are plugged in)

*/
#include <Servo.h> 
const int power = 5;
const int led = 11;
Servo servoFront;
Servo servoBack;

long duration;
int distance;
int stop = 90;

void setup() {
  Serial.begin(9600);

  servoFront.attach(9, 1000, 2000);
  servoBack.attach(10, 1000, 2000);

  pinMode(led, OUTPUT); //LED pin declerartion
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
    digitalWrite(led,HIGH);
    servoFront.write(98);
    servoBack.write(98);
    Serial.println("Forward");

    delay(3000);
    digitalWrite(led,LOW);
    servoFront.write(stop);
    servoBack.write(stop);
    Serial.println("Servo reset");
    Serial.println(stop);
    Serial.println("Break 2");

    delay(10000);
    digitalWrite(11,HIGH);
    servoFront.write(70);
    servoBack.write(70);
    Serial.println("Reverse");

    delay(3000);
    digitalWrite(led,LOW);
    servoFront.write(stop);
    servoBack.write(stop);
    Serial.println("Servo reset");
    delay(10000);
    digitalWrite(power,LOW);
    if(stop <= 96){
      stop = stop+1;
      Serial.println(stop);
      Serial.println("Break");
    }
  }
}