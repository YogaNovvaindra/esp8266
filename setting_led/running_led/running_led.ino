// Define the pins for the LEDs and buttons
const int ledD1 = D1; // D1 LED
const int ledD2 = D2; // D2 LED
const int ledD3 = D3; // D3 LED
const int ledD4 = D4; // D4 LED

const int buttonD5 = D5; // D5 button
const int buttonD6 = D6; // D6 button

// Flags to track LED states
bool leds12On = false;
bool leds34On = false;

void setup() {
  // Initialize LEDs as OUTPUT
  pinMode(ledD1, OUTPUT);
  pinMode(ledD2, OUTPUT);
  pinMode(ledD3, OUTPUT);
  pinMode(ledD4, OUTPUT);

  // Initialize buttons as INPUT with pull-up resistors
  pinMode(buttonD5, INPUT_PULLUP);
  pinMode(buttonD6, INPUT_PULLUP);

  // Initialize the serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the state of button D5
  int stateD5 = digitalRead(buttonD5);
  // Check if button D5 is pressed
  if (stateD5 == LOW) {
    digitalWrite(ledD1, HIGH); // Turn on D1 LED
    digitalWrite(ledD2, HIGH); // Turn on D2 LED
    leds12On = true;
  } else {
    digitalWrite(ledD1, LOW); // Turn off D1 LED
    digitalWrite(ledD2, LOW); // Turn off D2 LED
    leds12On = false;
  }

  // Read the state of button D6
  int stateD6 = digitalRead(buttonD6);
  // Check if button D6 is pressed
  if (stateD6 == LOW) {
    digitalWrite(ledD3, HIGH); // Turn on D3 LED
    digitalWrite(ledD4, HIGH); // Turn on D4 LED
    leds34On = true;
  } else {
    digitalWrite(ledD3, LOW); // Turn off D3 LED
    digitalWrite(ledD4, LOW); // Turn off D4 LED
    leds34On = false;
  }

  // Print the appropriate message based on LED states
  if (leds12On) {
    Serial.println("Lampu 1 dan 2 hidup");
  } else {
    Serial.println("Lampu 1 dan 2 mati");
  }

  if (leds34On) {
    Serial.println("Lampu 3 dan 4 hidup");
  } else {
    Serial.println("Lampu 3 dan 4 mati");
  }

  delay(100); // Add a small delay to debounce the buttons
}
