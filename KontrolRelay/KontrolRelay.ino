//Include library untuk koneksi ke WIfi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//konfigurasi wifi
const char* ssid = "Agim";
const char* password = "gakdipassword";

//alamat IP Address Server
String IPAddr ="dehidrasi.yoganova.my.id";
// String IPAddr ="172.20.10.9";

//Siapkan object client
WiFiClient NodeMCU;

//Siapkan pin untuk LED
#define PinRelay 5 // GPIO14 = D5

void setup() {
//Aktifkan Serial
Serial.begin(9600);

//koneksi WiFI
WiFi.hostname("NodeMCU");
WiFi.begin(ssid, password);

//uji status koneksi WiFI
while(WiFi.status() != WL_CONNECTED)
{
  //koneksi terus
  Serial.println(".");
  delay(500);
}
// Apabila sudah konek
Serial.println("Berhasil Konek");
pinMode(PinRelay, OUTPUT);
digitalWrite(PinRelay, 0);
//apabila terkoneksi nyalakan lampu LED
//digitalWrite(PinLED, 1);

}

void loop() {

String  LinkRelay;
HTTPClient httpRelay;
LinkRelay = "http://"+String(IPAddr)+"/kontroling/bacarelay.php";
httpRelay.begin(NodeMCU, LinkRelay);
//ambil isi status relay
httpRelay.GET();
//baca response
String responseRelay = httpRelay.getString();
Serial.println(responseRelay.toInt());
httpRelay.end();

// digitalWrite(PinRelay, HIGH);
}
