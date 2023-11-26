#include <Arduino_FreeRTOS.h>
#include <semphr.h>
// #include "BTP.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example
// void changeDisplay();

const int reset_pin = 13;

// Define pins for ultrasonic sensor
#define TRIGGER_PIN 9
#define ECHO_PIN 10

// Task handles
TaskHandle_t ultrasonicTaskHandle = NULL;
TaskHandle_t movePodHandle = NULL;
TaskHandle_t oledHandle = NULL;


// Semaphore for data protection
SemaphoreHandle_t ultrasonicDataMutex;

// Global variable to store ultrasonic sensor data
volatile long ultrasonicDistance = 0;

// Function prototypes
void ultrasonicTask(void *pvParameters);
void movePod(void *pvParameters);
void oledDisp(void *pvParameters);

long readUltrasonicSensor();

void setup() {
  Serial.begin(9600);                           //Set baudrate to 9600
  Serial.println("Setups");

  // //Servo attachment
  // servoLeft.attach(left_servos, 1000, 2000);    //Attaches the motor driver on pin 9 to the servoLeft object (pin, min pulse width, max pulse width in microseconds)
  // servoRight.attach(right_servos, 1000, 2000);  //Attaches the motor driver on pin 10 to the servoRight object (pin, min pulse width, max pulse width in microseconds)

  // //Pin IO Setup
  // pinMode(blue_led, OUTPUT);                    //LED pin declerartion as OUTPUT
  // pinMode(green_led, OUTPUT);                   //LED pin declerartion as OUTPUT
  // pinMode(trigFront, OUTPUT);                   //trigFront pin declerartion as OUTPUT
  // pinMode(echoFront, INPUT);                    //echoFront pin declerartion as OUTPUT
  // pinMode(trigBack, OUTPUT);                    //trigFront pin declerartion as OUTPUT
  // pinMode(echoBack, INPUT);                     //echoFront pin declerartion as OUTPUT
  // pinMode(power, OUTPUT);                       //power pin declerartion as OUTPUT
  // pinMode(reset_pin, INPUT);
  // //LED start up
  // digitalWrite(blue_led,HIGH);                  //Turn on LED for set up
  // digitalWrite(green_led,HIGH);                 //Turn on LED for reverse movement state
  // digitalWrite(power, HIGH);                    //Turn on power for set up

  // //ESC safety start up
  // servoLeft.write(midpoint);                    //Set left side motors postion to 90
  // servoRight.write(midpoint);                   //Set right side motors postion to 90
  // delay(5000);                                  //Delay for 5 seconds
  // digitalWrite(blue_led,LOW);                   //Turn off LED for set up
  // digitalWrite(green_led,LOW);                  //Turn on LED for reverse movement state
  // direction = 'S';                              //Set direction var to forward (BVM)

  // //default_mode(distanceBack);                   //Reset Pod to the start point (GCS)
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally

  Serial.println("pasqwerty");
  // // Show initial display buffer contents on the screen --
  // // the library initializes this with an Adafruit splash screen.

  // Initialize FreeRTOS
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ultrasonicDataMutex = xSemaphoreCreateMutex();
  display.clearDisplay();
  // Create ultrasonic sensor task
  xTaskCreate(ultrasonicTask, "UltrasonicTask", 128, NULL, 2, &ultrasonicTaskHandle);
  xTaskCreate(movePod, "movePod", 128, NULL, 1, &movePodHandle);
  xTaskCreate(oledDisp, "oledDisp", 128, NULL, 3, &oledHandle);
  Serial.println("padsss");  
}

void changeDisplay(){
  long distance = getUltrasonicDistance();
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);
  display.print(F("Dist to BVM: "));
  display.print(distance);
  display.setCursor(0,12);
  display.print(F("Dist to GCS: "));
  // display.print(distanceBack);
  display.setCursor(0,24); 
  display.print(F("Direction: "));
  // display.print(direction);
  display.display();
}


void loop() {
  // Your main loop code (if needed)
}
void oledDisp(void *pvParameters) {
  (void)pvParameters;
  changeDisplay();
  vTaskDelay(1000 / portTICK_PERIOD_MS );
}
void ultrasonicTask(void *pvParameters) {
  (void)pvParameters;

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  for (;;) {
    Serial.println("Read");
    // Read ultrasonic sensor
    long distance = readUltrasonicSensor();

    // Acquire the semaphore to protect shared data
    if (xSemaphoreTake(ultrasonicDataMutex, portMAX_DELAY) == pdTRUE) {
      ultrasonicDistance = distance;
      // Release the semaphore
      xSemaphoreGive(ultrasonicDataMutex);
    }

    // Print the distance
    Serial.print("Distance: ");
    Serial.print(ultrasonicDistance);
    Serial.println(" cm");

    // Add a delay to control the task execution rate
    delay(1000);
  }
}

long readUltrasonicSensor() {
  // Send a pulse to trigger the ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Read the echo duration
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance based on the speed of sound
  long distance = duration * 0.034 / 2;
  // Serial.println(distance);
  return distance;
}

long getUltrasonicDistance() {
  long distance;

  // Acquire the semaphore to protect shared data
  if (xSemaphoreTake(ultrasonicDataMutex, portMAX_DELAY) == pdTRUE) {
    distance = ultrasonicDistance;
    // Release the semaphore
    xSemaphoreGive(ultrasonicDataMutex);
  }

  return distance;
}

void movePod(void *pvParameters) {
  (void)pvParameters;
  for(;;) {
    Serial.println(getUltrasonicDistance());
  //   while(digitalRead(reset_pin) == LOW);  
  //   distance = getUltrasonicDistance();
  //   distanceBack = getUltrasonicDistance();
  //   oledDisplay();

  //   if (Serial.available()>0) {
  //     int mode = Serial.read();
  //     switch(mode) {
  //       case 'b':                       //Mode 1: Pod moving to the right
  //         direction = 'B';        
  //         break;
  //       case 'g':                       //Mode 2: Pod moving to the left
  //         direction = 'G';
  //         break;
  //       default:
  //         break;
  //     }
  //   }


  //   if(direction == 'G')                          //Check if direction is set as forward
  //   {
  //     digitalWrite(blue_led,HIGH);                //Turn on LED for forward movement state
  //     digitalWrite(green_led,LOW);                //Turn on LED for reverse movement state
  //     while(true){
  //       distance = getUltrasonicDistance();
  //       distanceBack = getUltrasonicDistance();
  //       oledDisplay();
  //       gradientControl(forward_speed);
  //       if(distanceBack < 20)                       //Check if the distance is less than or equal to 10 cm  from the front
  //       {
  //         direction = 'S';                          //Set direction to none to trigger a stop
  //         Serial.write('1');
  //         break;
  //       }
  //       delay(50);
  //       gradientControl(midpoint);                   //Set right side motors postion to 90
  //     }
  //   }
  //   else if(direction == 'B')                     //Check if direction is set as backwards (GCS)
  //   {   
  //     digitalWrite(blue_led,LOW);                 //Turn on LED for reverse movement state
  //     digitalWrite(green_led,HIGH);               //Turn on LED for reverse movement state   
  //     while(true){
  //       distance = getUltrasonicDistance();
  //       distanceBack = getUltrasonicDistance();
  //       oledDisplay();
  //       gradientControl(reverse_speed);
  //       //reverse(reverse_speed);
  //       if(distance < 20)                           //Check if the distance is less than or equal to 10 cm from the back
  //       {
  //         is_reverse = false;
  //         direction = 'S';                          //Set direction to none to trigger a stop
  //         Serial.write('1');
  //         break;
  //       }
  //       delay(50);
  //       gradientControl(midpoint);                   //Set right side motors postion to 90
  //     }
  //   }
  //   else if(direction == 'S'){                                         //Check if direction not forward or backward
  //     digitalWrite(blue_led,LOW);                 //Turn on LED for reverse movement state
  //     digitalWrite(green_led,LOW);               //Turn on LED for reverse movement state
  //     servoLeft.write(midpoint);                    //Set left side motors postion to 90
  //     servoRight.write(midpoint);                   //Set right side motors postion to 90
  //   }
  }
}
