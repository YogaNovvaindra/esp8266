const int ledPins[] = {D1, D2, D3, D4};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
const int buttonPin1 = D5; // Button connected to D5
const int buttonPin2 = D6; // Button connected to D6

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(buttonPin1, INPUT_PULLUP); // Set D5 as input with internal pull-up resistor
  pinMode(buttonPin2, INPUT_PULLUP); // Set D6 as input with internal pull-up resistor
}

void loop() {
  // Check if button 1 (D5) is pressed
  if (digitalRead(buttonPin1) == LOW) {
    // Turn on LEDs on D1 and D2
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
  } else {
    // Turn off LEDs on D1 and D2
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
  }

  // Check if button 2 (D6) is pressed
  if (digitalRead(buttonPin2) == LOW) {
    // Turn on LEDs on D3 and D4
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
  } else {
    // Turn off LEDs on D3 and D4
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
  }
}
