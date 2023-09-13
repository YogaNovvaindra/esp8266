//Include library untuk koneksi ke WIfi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//inisialisasi pin sensor
#define triggerPin 5 // Pin D1
#define echoPin 4     // Pin D2

// siapkan variabel
long durasi;
int jarak; 


//konfigurasi wifi
const char* ssid = "Agim";
const char* password = "gakdipassword";

//IP Server tempat aplikasi berjalan
const char* server = "dehidrasi.yoganova.my.id";

//pin untuk lampu LED
#define ledPin 2 // pin D4 9(belum dipasang)

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  //koneksi ke WIFI
  WiFi.hostname ("NodeMCU");
  WiFi.begin(ssid, password);
  // Uji Koneksi WiFi
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(ledPin, LOW);
    delay(500);
  }
  //Apabila sdh terkoneksi
  digitalWrite(ledPin, HIGH);
}

void loop() {
 //Membaca nilai jarak Sensor Ultrasonik
 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);
 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerPin, LOW);

 //baca durasi waktu gelombang dipancarkan
 durasi = pulseIn(echoPin, HIGH);
 //hitung jarak dalam cm
 jarak = durasi * 0.034 / 2; //menghasilkan jarak dalam cm
 Serial.println(jarak);

 // kirim data ke database
 WiFiClient client;
 String Link ;
 HTTPClient http;
 Link ="http://"+String(server)+"/waterlevel/kirimdata.php?tinggi="+String(jarak);
 http.begin(client, Link);
 http.GET();
 http.end();

 
 delay(1000);
}
