#include <WiFi.h>
#include <ThingSpeak.h>

// WiFi
const char* ssid = "project";
const char* password = "project1234";

WiFiClient client;
unsigned long channelNumber = 3355596;
const char * writeAPIKey = "50PRNFQ0JLXB6FKU";

// Pins
const int ldrPin = 36;
const int tempPin = 39;
const int motorPin1 = 22;
const int motorPin2 = 23;

// Thresholds
int ldrThreshold = 2000;
float tempThreshold = 30.0;

void setup() {
  Serial.begin(115200);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");

  ThingSpeak.begin(client);
}

void loop() {

  // Read LDR
  int ldrValue = analogRead(ldrPin);

  // Read Temperature (LM35)
  int sensorValue = analogRead(tempPin);
  float voltage = sensorValue * (3.3 / 4095.0);
  float temperature = voltage * 100.0;

  Serial.print("LDR: ");
  Serial.println(ldrValue);

  Serial.print("Temp: ");
  Serial.println(temperature);
// MAIN CONDITION
  if (ldrValue > ldrThreshold && temperature > tempThreshold) {

    Serial.println("Motor ON");

    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);

  } else {

    Serial.println("Motor OFF");

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }

  // ThingSpeak update
  static unsigned long lastUpdate = 0;

  if (WiFi.status() == WL_CONNECTED && millis() - lastUpdate > 15000) {

    ThingSpeak.setField(1, ldrValue);
    ThingSpeak.setField(2, temperature);

    int response = ThingSpeak.writeFields(channelNumber, writeAPIKey);

    Serial.print("Response: ");
    Serial.println(response);

    lastUpdate = millis();
  }

  delay(500);
}
