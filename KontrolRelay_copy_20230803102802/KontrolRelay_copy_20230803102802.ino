// Include library untuk koneksi ke WIfi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

// konfigurasi wifi
const char *ssid = "Agim";
const char *password = "gakdipassword";

// alamat IP Address Server
String host = "dehidrasi.yoganova.my.id";

#define pin_relay 16 // D0

void setup()
{
  Serial.begin(9600);

  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Berhasil Konek");

  pinMode(pin_relay, OUTPUT);
}

void loop()
{
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Koneksi Gagal");
    return;
  }

  String LinkRelay;
  HTTPClient httpRelay;
  LinkRelay = "http://" + String(host) + "/kontroling/bacarelay.php";
  httpRelay.begin(client, LinkRelay);
  httpRelay.GET();
  String responseRelay = httpRelay.getString();
  Serial.println(responseRelay);
  httpRelay.end();

  // digitalWrite(pin_relay, responseRelay.toInt());

  if (responseRelay.equals("1"))
  {
    Serial.println("Relay ON");
    digitalWrite(pin_relay, 0);
  }
  else if (responseRelay.equals("0"))
  {
    Serial.println("Relay OFF");
    digitalWrite(pin_relay, 1);
  }
  else
  {
    Serial.println("Unexpected value: " + responseRelay);
  }
}