// Include library untuk koneksi ke WIfi
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

// inisialisasi pin sensor
#define triggerPin 5	  // Pin D1
#define echoPin 4		  // Pin D2
const int S0 = 0;		  // D3
const int S1 = 2;		  // D4
const int S2 = 14;		  // D5
const int S3 = 12;		  // D6
const int sensorOut = 13; // D7

// variable sensor jarak
long durasi;
int jarak;

// begin variable sensor rgb
//  Calibration Values
//  *Get these from Calibration Sketch
int redMin = 250;	// Red minimum value
int redMax = 900;	// Red maximum value
int greenMin = 350; // Green minimum value
int greenMax = 900; // Green maximum value
int blueMin = 350;	// Blue minimum value
int blueMax = 768;	// Blue maximum value

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Variables for final Color values
int redValue;
int greenValue;
int blueValue;
// end variable rgb

// konfigurasi wifi
const char *ssid = "Agim";
const char *password = "gakdipassword";

// IP Server tempat aplikasi berjalan
const char *server = "dehidrasi.yoganova.my.id";

// pin untuk lampu LED
#define ledPin 16 // pin D0(belum dipasang)

void setup()
{
	Serial.begin(9600);
	pinMode(echoPin, INPUT);
	pinMode(triggerPin, OUTPUT);
	pinMode(ledPin, OUTPUT);

	// koneksi ke WIFI
	WiFi.hostname("NodeMCU");
	WiFi.begin(ssid, password);
	// Uji Koneksi WiFi
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		digitalWrite(ledPin, LOW);
		delay(500);
	}
	// Apabila sdh terkoneksi
	digitalWrite(ledPin, HIGH);

	// begin setup sensor rgb
	//  Set S0 - S3 as outputs
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);

	// Set Sensor output as input
	pinMode(sensorOut, INPUT);

	// Set Frequency scaling to 20%
	digitalWrite(S0, HIGH);
	digitalWrite(S1, LOW);
	// begin setup sensor rgb
}

void loop()
{
	// begin sensor ultrasonic
	digitalWrite(triggerPin, LOW);
	delayMicroseconds(2);
	digitalWrite(triggerPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(triggerPin, LOW);

	// baca durasi waktu gelombang dipancarkan
	durasi = pulseIn(echoPin, HIGH);
	// hitung jarak dalam cm
	jarak = durasi * 0.034 / 2; // menghasilkan jarak dalam cm
  Serial.println("Jarak: ");
	Serial.println(jarak);
	// end sensor ultrasonic

	// begin sensor kekeruhan
	int sensorValue = analogRead(0);
	// mapping nilai sensor
	int kekeruhan = map(sensorValue, 0, 750, 0, 100);

	// Tampilkan nilai di serial monitor;
  Serial.println("Kekeruhan: ");
	Serial.println(kekeruhan);
	// end sensor kekeruhan

	// begin sensor rgb
	//  Read Red value
	redPW = getRedPW();
	// Map to value from 0-255
	redValue = map(redPW, redMin, redMax, 255, 0);
	// Delay to stabilize sensor
	delay(200);

	// Read Green value
	greenPW = getGreenPW();
	// Map to value from 0-255
	greenValue = map(greenPW, greenMin, greenMax, 255, 0);
	// Delay to stabilize sensor
	delay(200);

	// Read Blue value
	bluePW = getBluePW();
	// Map to value from 0-255
	blueValue = map(bluePW, blueMin, blueMax, 255, 0);
	// Delay to stabilize sensor
	delay(200);

	// Cap the values at a maximum of 255
	if (redValue > 255)
	{
		redValue = 255;
	}
	if (greenValue > 255)
	{
		greenValue = 255;
	}
	if (blueValue > 255)
	{
		blueValue = 255;
	}

	// Set the values to a minimum of 0
	if (redValue < 0)
	{
		redValue = 0;
	}
	if (greenValue < 0)
	{
		greenValue = 0;
	}
	if (blueValue < 0)
	{
		blueValue = 0;
	}

	// Print output to Serial Monitor
	Serial.print("Red = ");
	Serial.print(redValue);
	Serial.print(" - Green = ");
	Serial.print(greenValue);
	Serial.print(" - Blue = ");
	Serial.println(blueValue);
	// end sensor rgb

	// kirim data ke database
	WiFiClient client;
	String Link;
	HTTPClient http;
	Link = "http://" + String(server) + "/api/data.php?tinggi=" + String(jarak) + "&kekeruhan=" + String(kekeruhan) + "&r=" + String(redValue) + "&g=" + String(greenValue) + "&b=" + String(blueValue);
  // https://dehidrasi.yoganova.my.id/api/data.php?sensor=10&tinggi=10&r=10&g=10&b=10
	http.begin(client, Link);
	http.GET();
	http.end();
	delay(1000);
}

// Function to read Red Pulse Widths
int getRedPW()
{
	// Set sensor to read Red only
	digitalWrite(S2, LOW);
	digitalWrite(S3, LOW);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Green Pulse Widths
int getGreenPW()
{
	// Set sensor to read Green only
	digitalWrite(S2, HIGH);
	digitalWrite(S3, HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}

// Function to read Blue Pulse Widths
int getBluePW()
{
	// Set sensor to read Blue only
	digitalWrite(S2, LOW);
	digitalWrite(S3, HIGH);
	// Define integer to represent Pulse Width
	int PW;
	// Read the output Pulse Width
	PW = pulseIn(sensorOut, LOW);
	// Return the value
	return PW;
}
