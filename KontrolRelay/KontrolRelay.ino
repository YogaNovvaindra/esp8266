#define PinRelay 4   // GPIO14 = D5
#define ButtonPin 2  // GPIO2 = D4

int condition = 0; // Initial condition is 0 (off)

void setup() {
  // Aktifkan Serial
  Serial.begin(9600);

  pinMode(PinRelay, OUTPUT);
  digitalWrite(PinRelay, HIGH); // Set the relay to off initially

  pinMode(ButtonPin, INPUT_PULLUP); // Configure the button pin with pull-up resistor
}

void loop() {
  // Check if the button on GPIO 2 is pressed (button press will read LOW due to the pull-up resistor)
  if (digitalRead(ButtonPin) == LOW) {
    // Button is pressed, toggle the condition
    condition = 1 - condition; // Toggle between 0 and 1
    Serial.print("Condition is now: ");
    Serial.println(condition);
    delay(500); // Add a small delay to debounce the button (adjust as needed)
  }

  // Check the condition and control the relay accordingly
  if (condition == 0) {
    // If the condition is 0, turn off the relay
    digitalWrite(PinRelay, LOW); // Relay off
    Serial.println(condition);
  } else {
    // If the condition is 1, turn on the relay
    digitalWrite(PinRelay, HIGH); // Relay on
    Serial.println(condition);
  }

  // You can add more code here for other tasks in your loop
}
