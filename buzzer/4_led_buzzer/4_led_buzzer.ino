const int pushButtonPin = 3; // Pin for Pushbutton
const int ledPins[] = {6, 7, 8, 9}; // Pins for LEDs (from left to right)
const int buzzerPin = 10; // Pin for buzzer

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

    // Loop to move the LED from left to right
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledPins[i], HIGH); // Turn on the current LED
      // Play a buzzer sound
      tone(buzzerPin, 1000); // Buzzer frequency (e.g., 1000Hz)
      delay(500); // Delay for LED on time
      noTone(buzzerPin); // Turn off the buzzer
      digitalWrite(ledPins[i], LOW); // Turn off the current LED

      // Delay for transition effect (you can adjust the delay time)
      delay(200);
    }
    
    // Reset LEDs to off state
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }

  // Reset buttonPressed when the button is released
  if (buttonState == HIGH) {
    buttonPressed = false;
  }
}