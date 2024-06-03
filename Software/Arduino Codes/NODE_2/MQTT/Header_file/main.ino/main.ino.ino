#include "SensorSystem.h"

const char* ssid = "FRITZ!Box 7510 XW";
const char* password = "46018957035297244418";
const char* mqttServer = "mqtt.example.com";
const int mqttPort = 1883;
const char* mqttUser = "yourMQTTUser";
const char* mqttPassword = "yourMQTTPassword";

SensorSystem sensorSystem(ssid, password, mqttServer, mqttPort, mqttUser, mqttPassword);

void setup() {
    sensorSystem.begin();
}

void loop() {
    sensorSystem.loop();
}
