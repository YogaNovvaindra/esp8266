#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

// Define the pin connections
#define pin_relay 14 // D5
#define triggerPin 5 // D1
#define echoPin 4    // D2
#define ledPin 2     // pin D4

long durasi;
int jarak, aturan;

// konfigurasi wifi
const char *ssid = "SMK TRUNOJOYO";
const char *password = "tanyamasoki";

// IP Server tempat aplikasi berjalan
const char *server = "dehidrasi.yoganova.my.id";

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  // koneksi ke WIFI
  WiFi.hostname("Relay_8266");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(ledPin, HIGH);
    delay(500);
  }
  // Apabila sdh terkoneksi
  digitalWrite(ledPin, LOW);

  // Set S0 - S3 as outputs
  pinMode(pin_relay, OUTPUT);
  digitalWrite(pin_relay, HIGH);
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
}

void loop()
{
  // kirim data ke database
  WiFiClient client;
  String Link;
  HTTPClient http;
  Link = "http://" + String(server) + "/kontroling/bacarelay.php";
  http.begin(client, Link);
  http.GET();
  String responseRelay = http.getString();
  http.end();

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

  if (digitalRead(pin_relay) == LOW)
  {
    // get data jarak ketinggian
    WiFiClient clientgetjarak;
    String Linkgetjarak;
    HTTPClient httpgetjarak;
    Linkgetjarak = "http://" + String(server) + "/waterlevel/getaturan.php";
    httpgetjarak.begin(clientgetjarak, Linkgetjarak);
    httpgetjarak.GET();
    String atur = httpgetjarak.getString();
    Serial.println("Aturan: " + atur);
    aturan = atur.toInt();
    if (jarak == aturan | jarak <= aturan)
    {
      Serial.println("Relay OFF");
      digitalWrite(pin_relay, HIGH);
      WiFiClient clientsetkontrol;
      String Linksetkontrol;
      HTTPClient httpsetkontrol;
      Linksetkontrol = "http://" + String(server) + "/kontroling/setkontrol.php?kontrol=0";
      httpsetkontrol.begin(clientsetkontrol, Linksetkontrol);
      httpsetkontrol.GET();
      httpsetkontrol.end();
    }
    else
    {
      Serial.println("Mengisi..."); 
    }
    httpgetjarak.end();
  }
  else
  {
  }
  // Send data to the server
  WiFiClient clientSensor;
  String LinkSensor;
  HTTPClient httpSensor;
  LinkSensor = "http://" + String(server) + "/waterlevel/kirimdata.php?tinggi=" + String(jarak);
  httpSensor.begin(clientSensor, LinkSensor);
  httpSensor.GET();
  httpSensor.end();

  delay(1000);
}
