#include "SensorSystem.h"

// Wi-Fi and MQTT credentials
const char* ssid = "FRITZ!Box 7510 XW"; // Network SSID
const char* password = "46018957035297244418"; // Network password
const char* mqttServer = "mqtt.example.com"; // MQTT server address
const int mqttPort = 1883; // MQTT server port
const char* mqttUser = "yourMQTTUser"; // MQTT username
const char* mqttPassword = "yourMQTTPassword"; // MQTT password

// Create a SensorSystem object with the above credentials
SensorSystem sensorSystem(ssid, password, mqttServer, mqttPort, mqttUser, mqttPassword);

void setup() {
    sensorSystem.begin(); // Initialize the sensor system
}

void loop() {
    sensorSystem.loop(); // Continuously run the sensor system
}
