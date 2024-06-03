#ifndef NODE_2_H
#define NODE_2_H

#include <WiFiNINA.h>

// WiFi credentials
extern const char* ssid;
extern const char* password;

// Create a WiFi server object
extern WiFiServer server;

// Sensor pins
extern const int flameAnalogPin;
extern const int photoresistorPin;
extern const int trigPin;
extern const int echoPin;
extern const int greenLedPin;
extern const int redLedPin;
extern const int relayPin;
extern const int buzzerPin;
extern const int tempAnalogPin;

// Function declarations
void connectWiFi();
long measureDistance();
void initializePins();

#endif
