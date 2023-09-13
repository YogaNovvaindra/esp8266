//Include library untuk koneksi ke WIfi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//konfigurasi wifi
const char* ssid = "Agim";
const char* password = "gakdipassword";

//alamat IP Address Server
String IPAddr ="dehidrasi.yoganova.my.id";

//Siapkan object client
WiFiClient NodeMCU;


//Siapkan pin untuk LED
#define PinLED 5 // GPIO5 = D1


void setup() {
//Aktifkan Serial
Serial.begin(9600);
//mode Pin LED
pinMode(PinLED, OUTPUT);
//koneksi WiFI
WiFi.begin(ssid, password);
//uji status koneksi WiFI
while(WiFi.status() != WL_CONNECTED)
{
  digitalWrite(PinLED, LOW);
  delay(500);
}

//apabila terkoneksi nyalakan lampu LED
digitalWrite(PinLED, HIGH);

}

void loop() {
//pastikan koneksi web server berhasil
if(!NodeMCU.connect(IPAddr, 80))
{
  Serial.println("Gagal Koneksi ke WEB SERVER");
  return ;
}



int sensorValue = analogRead(0);
//mapping nilai sensor
int kekeruhan = map(sensorValue, 0, 750, 0, 100);

//Tampilkan nilai di serial monitor;
Serial.println(kekeruhan);

//kirim data ke database / aplikasi website
HTTPClient http;
String url="http://"+IPAddr+"/monitoringair/kirimdata.php?sensor=" + String(kekeruhan);

//eksekusi link URL
http.begin(NodeMCU, url);

//mode GET
http.GET();
//akhiri proses kirim data
http.end();

delay(500);
}
