#ifndef MQTTHandler_h
#define MQTTHandler_h

#include <WiFiNINA.h>
#include <PubSubClient.h>

extern WiFiClient wifiClient;
extern PubSubClient mqttClient;

void connectWiFi(const char* ssid, const char* password);
void connectMQTT(const char* mqtt_server, int mqtt_port, const char* mqtt_username, const char* mqtt_password, const char* mqtt_client_id);

#endif
