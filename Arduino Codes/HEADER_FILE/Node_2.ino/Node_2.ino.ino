#include <WiFiNINA.h>
#include "MQTTHandler.h"

// WiFi credentials
const char* ssid = "FRITZ!Box 7510 XW";
const char* password = "46018957035297244418";

// MQTT Broker details
const char* mqtt_server = "broker.example.com";
const int mqtt_port = 1883;
const char* mqtt_username = "your_mqtt_username";
const char* mqtt_password = "your_mqtt_password";
const char* mqtt_client_id = "arduino_client";

void setup() {
  Serial.begin(115200);
  connectWiFi(ssid, password);
  connectMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_password, mqtt_client_id);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi(ssid, password);
  }
  if (!mqttClient.connected()) {
    connectMQTT(mqtt_server, mqtt_port, mqtt_username, mqtt_password, mqtt_client_id);
  }
  mqttClient.loop();
  // Your sensor reading and publishing code goes here
}
