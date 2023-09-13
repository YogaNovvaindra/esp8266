//include library WIFI nodemcu ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


//Network SSID
const char* ssid = "iPhone pc";
const char* pass = "dtrias1971";

//ambil IP address server
const char* host = "172.20.10.9";

void setup() {
    Serial.begin(9600);

    //koneksi ke WiFi
    WiFi.hostname("NodeMCU");
    WiFi.begin(ssid, pass);

    //cek koneksi apakah berhasil
  while(WiFi.status() != WL_CONNECTED) 
  {
    // coba koneksi terus
    Serial.println(".");
    delay(500);

  }
// Apabila sudah konek cetak berhasil
Serial.println("Berhasil Konek");
}

void loop() {
  // put your main code here, to run repeatedly:

}
