#include "DHTesp.h"
DHTesp dht;

const int sensor_pin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.setup(D1, DHTesp::DHT11);
}

void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.getHumidity();
  float t = dht.getTemperature();
  Serial.print("{\"Kelembaban\" : ");
  Serial.print(h);
  Serial.print(", \"Temperatur\" : ");
  Serial.print(t);
  Serial.print("}\n");
  delay(1000);
  //end dht

  //begin HW390
  float moist_persen;

  moist_persen = (100.00 - ((analogRead(sensor_pin) / 1023.00) * 100.00));

  Serial.print("tanah moist(%) = ");
  Serial.print(moist_persen);
  Serial.print("%");

  delay(1000);
}
