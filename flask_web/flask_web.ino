#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

// const char *ssid = "SMK TRUNOJOYO";
// const char *password = "tanyamasoki";
const char *ssid = "SKK - STUDENT";
const char *password = "sistemkomputerkontrol";
String host = "10.10.7.198";

#define pin_relay 16 // D0
#define triggerPin 5 // D1
#define echoPin 4    // D2
#define ledPin 2     // D4

long durasi;
int jarak;

void setup()
{
  Serial.begin(9600);

  WiFi.hostname("Ultrasonic_Relay_8266");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(ledPin, LOW);
    delay(500);
  }

  Serial.println("Berhasil Konek");
  digitalWrite(ledPin, HIGH);

  pinMode(pin_relay, OUTPUT);
  digitalWrite(pin_relay, HIGH);
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  // Relay Control
  WiFiClient client;
  const int httpPort = 5000;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Koneksi Gagal (Relay Control)");
    return;
  }

  String LinkRelay;
  HTTPClient httpRelay;
  LinkRelay = "http://" + String(host) +":5000" + "/get";
  httpRelay.begin(client, LinkRelay);
  httpRelay.GET();
  String responseRelay = httpRelay.getString();
  Serial.println(responseRelay);
  httpRelay.end();

  if (responseRelay.equals("1"))
  {
    Serial.println("Relay ON");
    digitalWrite(pin_relay, LOW);
  }
  else if (responseRelay.equals("0"))
  {
    Serial.println("Relay OFF");
    digitalWrite(pin_relay, HIGH);
  }
  else
  {
    Serial.println("Unexpected value: " + responseRelay);
  }

  // Ultrasonic Sensor
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  durasi = pulseIn(echoPin, HIGH);
  jarak = durasi * 0.034 / 2;
  Serial.println("Jarak: " + String(jarak) + " cm");

  // Send data to the server
  WiFiClient clientSensor;
  String LinkSensor;
  HTTPClient httpSensor;
  LinkSensor = "http://" + String(host) +":5000" + "/jarak?jarak=" + String(jarak);
  httpSensor.begin(clientSensor, LinkSensor);
  httpSensor.GET();
  httpSensor.end();

  delay(2000);
}
