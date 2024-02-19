// Ultrasonic Sensor testing code.
int trigPin = 15; // Add the Trig pin on pin 10.
int echoPin = 14; // Add the ECHO pin on pin 9.
int duration, distance; // Add types 'duration' and 'distance'.

void setup()
{
  pinMode (trigPin, OUTPUT);// Same as above, the TRIG pin will send the ultrasonic wave.
  pinMode (echoPin, INPUT); // The ECHO pin will recieve the rebounded wave, so it must be an input type.
  Serial.begin(9600);
}

void loop()
{
  digitalWrite (trigPin, HIGH);
  delay(50);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)/29.1;
  delay(50);
  Serial.print("cm");
  Serial.println(distance);
}