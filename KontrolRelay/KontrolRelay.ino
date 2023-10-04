#include <ESP8266WiFi.h>

// Define the GPIO pin that is connected to the relay
#define RELAY_PIN 16

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the relay pin as an output
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize the button pin as an input with a pull-down resistor
  pinMode(5, INPUT);
}

void loop() {
  // Read the state of the button
  int buttonState = digitalRead(4);

  // Write the opposite value to the relay pin
  digitalWrite(RELAY_PIN, !buttonState);

  // Print the button and relay states
  Serial.print("Button: ");
  Serial.print(buttonState);
  Serial.print(", Relay: ");
  Serial.println(!buttonState);

  // Wait for a short time
  delay(100);
}
