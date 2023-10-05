//Goal: have it crawl along the rail and respond to ultrasonic sensor
#include <Servo.h>
const int pwm = 6; // pin 6 as pwm
const int trigPinRight = 7;
const int echoPinRight = 8;
// const int trigPinLeft = 3;
// const int echoPinLeft = 4;

Servo servo;

int direction = 0;
long durationRight;
int distanceRight;
int currDistance = 0;
//int direction;

int readDistance(int direction){
  if(direction == 0){
    digitalWrite (trigPinRight, HIGH);
    delay(50);
    digitalWrite(trigPinRight, LOW);
    durationRight = pulseIn(echoPinRight, HIGH);
    distanceRight = (durationRight/2)/29.1;
  }
  else{
    digitalWrite (trigPinLeft, HIGH);
    delay(50);
    digitalWrite(trigPinLeft, LOW);
    durationLeft = pulseIn(echoPinLeft, HIGH);
    distanceLeft = (durationLeft/2)/29.1;
  }
}


void setup() {
  Serial.begin(9600);
  servo.attach(pwm, 1000, 2000);
  pinMode(pwm, OUTPUT); //declare pin pwm as OUTPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.write(95);
  delay(5000);
}

void loop() {
  currDistance = readDistance();
  if(currDistance < 50){
    servo.write(110);
  }
  else{
    servo.write(70);
  }

}
