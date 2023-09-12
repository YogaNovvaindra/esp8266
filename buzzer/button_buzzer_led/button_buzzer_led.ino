const int pushButtonPin = 2; // Pin for Pushbutton
// const int ledPins[] = {6, 7, 8, 9}; // Pins for LEDs (from left to right)
const int ledPins[] = {5, 6, 9, 10}; // Pins for LEDs (from left to right)
const int buzzerPin = 11; // Pin for buzzer

boolean buttonPressed = false;

void setup() {
  // Set pin-mode for pushButtonPin as input and LEDs as output
  pinMode(pushButtonPin, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Turn off all LEDs during setup
  }
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // Turn off buzzer during setup
}

void loop() {
  // Read the push button state
  int buttonState = digitalRead(pushButtonPin);

  // If the push button is pressed (logic is inverted due to INPUT_PULLUP)
  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    // Turn on all LEDs sequentially with a half-second delay between each LED
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(500); // Delay for half a second (500 milliseconds)
      digitalWrite(ledPins[i], LOW);
    }
    // Play a buzzer sound
    tone(buzzerPin, 1000); // Buzzer frequency (e.g., 1000Hz)
    delay(1000); // Play the buzzer sound for 1 second
    noTone(buzzerPin); // Turn off the buzzer
  }

  // Reset buttonPressed when the button is released
  if (buttonState == HIGH) {
    buttonPressed = false;
  }
}
