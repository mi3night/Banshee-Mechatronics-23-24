#include <Streaming.h>

void setup() {
  Serial.begin(115200);
  Serial.flush(); // Flush the serial buffer
}

void loop() {
  while (Serial.available()) {
    int incomingData;
    Serial >> incomingData;
    
    // Convert the received data to an integer
    // int incomingValue = incomingData.toInt();
    
    // Print the received value
    Serial << incomingData << endl;
    
    // Add your logic based on the received value
    // For example, control LEDs, motors, or other devices.
  }
}