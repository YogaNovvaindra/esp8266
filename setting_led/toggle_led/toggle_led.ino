const int ledPins[] = {D1, D2, D3, D4};
const int buttonPin1 = D5; // Tombol 1 untuk mengontrol lampu D1 dan D2
const int buttonPin2 = D6; // Tombol 2 untuk mengontrol lampu D3 dan D4

void setup() {
  Serial.begin(9600); // Mulai komunikasi serial dengan baud rate 9600

  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT); // Setiap pin LED sebagai OUTPUT
    digitalWrite(ledPins[i], LOW); // Matikan semua lampu saat inisialisasi
  }

  pinMode(buttonPin1, INPUT_PULLUP); // Set tombol 1 sebagai input dengan pull-up resistor
  pinMode(buttonPin2, INPUT_PULLUP); // Set tombol 2 sebagai input dengan pull-up resistor
}

void loop() {
  static int buttonState1 = HIGH; // Variabel untuk melacak status tombol 1
  static int buttonState2 = HIGH; // Variabel untuk melacak status tombol 2

  // Baca status tombol 1
  int newButtonState1 = digitalRead(buttonPin1);

  // Baca status tombol 2
  int newButtonState2 = digitalRead(buttonPin2);

  // Jika tombol 1 berubah dari HIGH menjadi LOW (ditekan)
  if (newButtonState1 == LOW && buttonState1 == HIGH) {
    // Toggle status lampu D1 dan D2
    digitalWrite(ledPins[0], !digitalRead(ledPins[0]));
    digitalWrite(ledPins[1], !digitalRead(ledPins[1]));
    
    // Cetak pesan ke Serial Monitor
    if (digitalRead(ledPins[0]) == HIGH && digitalRead(ledPins[1]) == HIGH) {
      Serial.println("Lampu D1 dan D2: Menyala");
    } else {
      Serial.println("Lampu D1 dan D2: Mati");
    }
  }

  // Jika tombol 2 berubah dari HIGH menjadi LOW (ditekan)
  if (newButtonState2 == LOW && buttonState2 == HIGH) {
    // Toggle status lampu D3 dan D4
    digitalWrite(ledPins[2], !digitalRead(ledPins[2]));
    digitalWrite(ledPins[3], !digitalRead(ledPins[3]));
    
    // Cetak pesan ke Serial Monitor
    if (digitalRead(ledPins[2]) == HIGH && digitalRead(ledPins[3]) == HIGH) {
      Serial.println("Lampu D3 dan D4: Menyala");
    } else {
      Serial.println("Lampu D3 dan D4: Mati");
    }
  }

  // Update status tombol 1 dan tombol 2
  buttonState1 = newButtonState1;
  buttonState2 = newButtonState2;
}