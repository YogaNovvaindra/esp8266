int Led1= 4;
int Led2=5;
int Led3=0;
int Led4=2;
void setup() {
  // put your setup code here, to run once:
pinMode(Led1, OUTPUT);
pinMode(Led2, OUTPUT);
pinMode(Led3, OUTPUT);
pinMode(Led4, OUTPUT);

Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(Led1, LOW);
digitalWrite(Led2, LOW);
digitalWrite(Led3, LOW);
digitalWrite(Led4, LOW);
Serial.println("LED OFF");
delay(1000);
digitalWrite(Led1, HIGH);
digitalWrite(Led2, HIGH);
digitalWrite(Led3, HIGH);
digitalWrite(Led4, HIGH);
Serial.println("LED ON");
delay(1000);
}