#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>

// Define the pin connections
const int S0 = 0;
const int S1 = 2;
const int S2 = 14;
const int S3 = 12;
const int sensorOut = 13;
#define ledPin 16 // pin D0

// Calibration Values
// *Get these from Calibration Sketch
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
// String Red="";
// String Green="";
// String Blue="";
// String Hydration="";
// String Centroid="";

// konfigurasi wifi
// const char *ssid = "SMK TRUNOJOYO";
// const char *password = "tanyamasoki";
const char *ssid = "Barokah";
const char *password = "gakdipassword";

// IP Server tempat aplikasi berjalan
const char *server = "dehidrasi.yoganova.my.id";

void setup()
{
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
	// koneksi ke WIFI
	WiFi.hostname("RGB_8266");
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		digitalWrite(ledPin, LOW);
		delay(500);
	}
	// Apabila sdh terkoneksi
	digitalWrite(ledPin, HIGH);

	// Set S0 - S3 as outputs
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);

	// Set Sensor output as input
	pinMode(sensorOut, INPUT);

	// Set Frequency scaling to 20%
	digitalWrite(S0, HIGH);
	digitalWrite(S1, LOW);
}

void loop()
{
	// Read Red value
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

	WiFiClient clientindikator;
	String Linkindikator;
	HTTPClient httpindikator;
	Linkindikator = "http://" + String(server) + "/rgbair/getindikator.php?";
	httpindikator.begin(clientindikator, Linkindikator);
	httpindikator.GET();
	String responseindikator = httpindikator.getString();

	if (responseindikator.equals("1"))
	{
		//kirim ke python
		WiFiClient clientfuzzy;
		String Linkfuzzy;
		HTTPClient httpfuzzy;
		Linkfuzzy = "http://fuzzy.yoganova.my.id/hydration?r=" + String(redValue) + "&g=" + String(greenValue) + "&b=" + String(blueValue);
		httpfuzzy.begin(clientfuzzy, Linkfuzzy);
		httpfuzzy.GET();
		// get data from json response
		String responsefuzzy = httpfuzzy.getString();
		JSONVar myObject = JSON.parse(responsefuzzy);
		int Red = myObject["Red"];
		int Green = myObject["Green"];
		int Blue = myObject["Blue"];
		// Hydration = myObject["Hydration"];
		double Centroid = myObject["Centroid"];
    // Serial.print("Red object = ");
    // Serial.print(Red);
		// kirim data ke database
		WiFiClient client;
		String Link;
		HTTPClient http;
		Link = "http://" + String(server) + "/rgbair/kirimdata.php?" + "r=" + String(Red) + "&g=" + String(Green) + "&b=" + String(Blue) + "&h=" + String(myObject["Hydration"]) + "&c=" + String(Centroid);
		// https://dehidrasi.yoganova.my.id/api/data.php?sensor=10&tinggi=10&r=10&g=10&b=10
    Serial.print(Link);
		http.begin(client, Link);
		http.GET();
		httpfuzzy.end();
		http.end();
		delay(2000);
	} else {
		Serial.println("Tidak ada permintaan");
	}
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
