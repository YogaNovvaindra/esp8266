#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

// WiFi configuration
const char *ssid = "Agim";
const char *password = "gakdipassword";
const char *server = "dehidrasi.yoganova.my.id";

// Ultrasonic sensor pins
#define triggerPin 5  // Pin D1
#define echoPin 4     // Pin D2

// RGB sensor pins
const int S0 = 0;    // D3
const int S1 = 2;    // D4
const int S2 = 14;   // D5
const int S3 = 12;   // D6
const int sensorOut = 13; // D7

// Relay control pin
#define relayPin 16// D0

// pin untuk lampu LED
#define ledPin 15 // pin D8(belum dipasang)

// Calibration Values for RGB sensor
int redMin = 250;
int redMax = 900;
int greenMin = 350;
int greenMax = 900;
int blueMin = 350;
int blueMax = 768;

WiFiClient client;

void setup()
{
  Serial.begin(9600);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  pinMode(relayPin, OUTPUT);

  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(ledPin, LOW); // Turn off relay while waiting for WiFi connection
    delay(500);
  }

  Serial.println("Connected to WiFi");
  digitalWrite(ledPin, HIGH); // Turn on relay when connected to WiFi
}

void loop()
{
  // Ultrasonic Sensor
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // RGB Sensor
  int redPW = getRedPW();
  int greenPW = getGreenPW();
  int bluePW = getBluePW();

  int redValue = map(redPW, redMin, redMax, 255, 0);
  int greenValue = map(greenPW, greenMin, greenMax, 255, 0);
  int blueValue = map(bluePW, blueMin, blueMax, 255, 0);

  // Cap the RGB values
  redValue = constrain(redValue, 0, 255);
  greenValue = constrain(greenValue, 0, 255);
  blueValue = constrain(blueValue, 0, 255);

  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  Serial.print(" - Blue = ");
  Serial.println(blueValue);

  // Relay Control
  String relayLink = "http://" + String(server) + "/kontroling/bacarelay.php";
  HTTPClient httpRelay;
  httpRelay.begin(client, relayLink);
  httpRelay.GET();
  String relayResponse = httpRelay.getString();
  httpRelay.end();

  if (relayResponse.equals("1"))
  {
    Serial.println("Relay ON");
    digitalWrite(relayPin, 0);
  }
  else if (relayResponse.equals("0"))
  {
    Serial.println("Relay OFF");
    digitalWrite(relayPin, 1);
  }
  else
  {
    Serial.println("Unexpected relay value: " + relayResponse);
  }

  // Send data to the server
  String sensorLink = "http://" + String(server) + "/api/data.php?distance=" + String(distance) + "&r=" + String(redValue) + "&g=" + String(greenValue) + "&b=" + String(blueValue);
  HTTPClient http;
  http.begin(client, sensorLink);
  http.GET();
  http.end();

  delay(1000); // Delay before the next iteration
}

// Functions to read RGB sensor values
int getRedPW()
{
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int PW = pulseIn(sensorOut, LOW);
  return PW;
}

int getGreenPW()
{
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int PW = pulseIn(sensorOut, LOW);
  return PW;
}

int getBluePW()
{
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int PW = pulseIn(sensorOut, LOW);
  return PW;
}
