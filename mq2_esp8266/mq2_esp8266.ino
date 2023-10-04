// Include the Wire library for I2C communication
#include <Wire.h>

// Define the I2C pins for ESP8266
#define SDA_PIN 5 // GPIO5
#define SCL_PIN 4 // GPIO4

// Define the analog pins for MQ-2 and MQ-135 sensors
#define MQ2_PIN A0 // ADC0
#define MQ135_PIN SDA // GPIO4

// Define the calibration values for MQ-2 and MQ-135 sensors
// You can adjust these values according to your sensor datasheet or experiment
#define MQ2_RO 10 // MQ-2 resistance in clean air
#define MQ135_RO 76.63 // MQ-135 resistance in clean air

// Define the constants for gas calculation formulas
// You can adjust these values according to your sensor datasheet or experiment
#define MQ2_LPG 0.42 // LPG curve slope
#define MQ2_CO 0.77 // CO curve slope
#define MQ2_SMOKE 0.42 // Smoke curve slope
#define MQ135_CO2 0.358 // CO2 curve slope
#define MQ135_CO 1.178 // CO curve slope
#define MQ135_NH4 0.47 // NH4 curve slope

// Define the serial baud rate
#define BAUD_RATE 9600

// Initialize the variables for storing sensor values
float mq2_volt = 0;
float mq2_rs = 0;
float mq2_ratio = 0;
float mq2_lpg = 0;
float mq2_co = 0;
float mq2_smoke = 0;

float mq135_volt = 0;
float mq135_rs = 0;
float mq135_ratio = 0;
float mq135_co2 = 0;
float mq135_co = 0;
float mq135_nh4 = 0;

void setup() {
  // Start the serial communication
  Serial.begin(BAUD_RATE);

  // Start the I2C communication with custom pins
  Wire.begin(SDA_PIN, SCL_PIN);
}

void loop() {
  // Read the analog value from MQ-2 sensor
  mq2_volt = analogRead(MQ2_PIN) * (5.0 / 1023.0);

  // Calculate the resistance of the sensor
  mq2_rs = (5.0 - mq2_volt) / mq2_volt;

  // Calculate the ratio of the sensor resistance in clean air
  mq2_ratio = mq2_rs / MQ2_RO;

  // Calculate the gas concentrations using the formulas from datasheet
  mq2_lpg = pow(10, ((log10(mq2_ratio) - log10(MQ2_LPG)) / (-MQ2_LPG)) + log10(MQ2_LPG));
  mq2_co = pow(10, ((log10(mq2_ratio) - log10(MQ2_CO)) / (-MQ2_CO)) + log10(MQ2_CO));
  mq2_smoke = pow(10, ((log10(mq2_ratio) - log10(MQ2_SMOKE)) / (-MQ2_SMOKE)) + log10(MQ2_SMOKE));

  // Read the analog value from MQ-135 sensor
  mq135_volt = analogRead(MQ135_PIN) * (5.0 / 1023.0);

  // Calculate the resistance of the sensor
  mq135_rs = (5.0 - mq135_volt) / mq135_volt;

  // Calculate the ratio of the sensor resistance in clean air
  mq135_ratio = mq135_rs / MQ135_RO;

  // Calculate the gas concentrations using the formulas from datasheet
  mq135_co2 = pow(10, ((log10(mq135_ratio) - log10(MQ135_CO2)) / (-MQ135_CO)) + log10(MQ135_CO));
  mq135_co = pow(10, ((log10(mq135_ratio) - log10(MQ135_CO)) / (-MQ135_CO)) + log10(MQ135_CO));
  mq135_nh4 = pow(10, ((log10(mq135_ratio) - log10(MQ135_NH4)) / (-MQ135_NH4)) + log10(MQ135_NH4));

  // Print the sensor values to serial monitor
  Serial.print("MQ-2 Sensor: ");
  Serial.print("LPG: ");
  Serial.print(mq2_lpg);
  Serial.print(" ppm, ");
  Serial.print("CO: ");
  Serial.print(mq2_co);
  Serial.print(" ppm, ");
  Serial.print("Smoke: ");
  Serial.print(mq2_smoke);
  Serial.println(" ppm");

  Serial.print("MQ-135 Sensor: ");
  Serial.print("CO2: ");
  Serial.print(mq135_co2);
  Serial.print(" ppm, ");
  Serial.print("CO: ");
  Serial.print(mq135_co);
  Serial.print(" ppm, ");
  Serial.print("NH4: ");
  Serial.print(mq135_nh4);
  Serial.println(" ppm");

  // Wait for a second before reading again
  delay(5000);
}
