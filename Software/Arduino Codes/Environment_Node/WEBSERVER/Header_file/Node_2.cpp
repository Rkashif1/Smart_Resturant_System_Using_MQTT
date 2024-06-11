#include "Node_2.h"

// WiFi credentials
const char* ssid = "FRITZ!Box 7510 XW";
const char* password = "46018957035297244418";

// Create a WiFi server object
WiFiServer server(80);

// Sensor pins
const int flameAnalogPin = A0;
const int photoresistorPin = A5;
const int trigPin = 9;
const int echoPin = 8;
const int greenLedPin = 11;
const int redLedPin = 12;
const int relayPin = 6;
const int buzzerPin = 7;
const int tempAnalogPin = A1;

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); 
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void initializePins() {
  pinMode(flameAnalogPin, INPUT);
  pinMode(photoresistorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT); 
  pinMode(tempAnalogPin, INPUT);
}
