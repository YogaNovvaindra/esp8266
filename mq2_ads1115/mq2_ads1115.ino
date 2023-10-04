// Include library
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Define constants
#define RL 5 // Load resistor value in kOhm
#define VC 5 // Circuit voltage in V
#define R0_CLEAN_AIR_FACTOR 9.83 // Sensor resistance in clean air / R0
#define CALIBRATION_SAMPLE_TIMES 50 // Number of samples for calibration
#define CALIBRATION_SAMPLE_INTERVAL 500 // Interval between samples for calibration in ms
#define READ_SAMPLE_TIMES 5 // Number of samples for reading
#define READ_SAMPLE_INTERVAL 50 // Interval between samples for reading in ms

// Define variables
float R0 = 0; // Sensor resistance in clean air
float RS = 0; // Sensor resistance in target gas
float VS = 0; // Sensor output voltage
float PPM = 0; // Gas concentration in PPM

// Create ADS1115 object
Adafruit_ADS1115 ads;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  Wire.begin(); // Initialize I2C communication
  ads.begin(); // Initialize ADS1115 module
}

void loop() {
  // Calibrate sensor in clean air
  Serial.print("Calibrating...");
  R0 = MQCalibration();
  Serial.println("Done!");
  Serial.print("R0 = ");
  Serial.println(R0);

  // Read sensor and calculate PPM
  Serial.print("Reading...");
  PPM = MQGetPPM(R0);
  Serial.println("Done!");
  Serial.print("PPM = ");
  Serial.println(PPM);

  delay(10000); // Wait for 10 seconds before next reading
}

// Function to calibrate sensor in clean air and get R0 value
float MQCalibration() {
  int i;
  float RS_air = 0;
  
  for (i = 0; i < CALIBRATION_SAMPLE_TIMES; i++) {
    RS_air += MQResistanceCalculation(ads.readADC_SingleEnded(0)); // Read and calculate RS value in clean air from ADC0 channel
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  
  RS_air = RS_air / CALIBRATION_SAMPLE_TIMES; // Get average RS value in clean air
  
  return RS_air / R0_CLEAN_AIR_FACTOR; // Calculate R0 value according to datasheet
}

// Function to read sensor and calculate PPM value
float MQGetPPM(float ro) {
  int i;
  float RS = 0;
  
  for (i = 0; i < READ_SAMPLE_TIMES; i++) {
    RS += MQResistanceCalculation(ads.readADC_SingleEnded(0)); // Read and calculate RS value in target gas from ADC0 channel
    delay(READ_SAMPLE_INTERVAL);
  }
  
  RS = RS / READ_SAMPLE_TIMES; // Get average RS value in target gas
  
  return MQPPMCalculation(RS / ro); // Calculate PPM value according to datasheet
}

// Function to calculate sensor resistance from analog value
float MQResistanceCalculation(int raw_adc) {
   VS = raw_adc * (VC / pow(2,15)); // Convert raw ADC value to voltage (ADS1115 has a resolution of 15 bits)
   return RL * ((VC - VS) / VS); 
}

// Function to calculate PPM from sensor resistance ratio
float MQPPMCalculation(float rs_ro_ratio) {
   return pow(10, (log10(rs_ro_ratio) - log10(1036.3)) / -2.186); 
}
