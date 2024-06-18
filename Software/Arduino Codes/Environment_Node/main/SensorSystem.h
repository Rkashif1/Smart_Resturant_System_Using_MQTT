#ifndef SENSOR_SYSTEM_H
#define SENSOR_SYSTEM_H

#include <WiFiNINA.h>
#include <PubSubClient.h>

class SensorSystem {
public:
    SensorSystem(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttUser, const char* mqttPassword);
    void begin();
    void loop();

private:
    void connectWiFi();
    void reconnect();
    long measureDistance();
    void publishSensorData();

    const char* ssid;
    const char* password;
    const char* mqttServer;
    int mqttPort;
    const char* mqttUser;
    const char* mqttPassword;

    WiFiClient wifiClient;
    PubSubClient client;

    const int flameAnalogPin = A0;
    const int photoresistorPin = A5;
    const int trigPin = 9;
    const int echoPin = 8;
    const int greenLedPin = 11;
    const int redLedPin = 12;
    const int relayPin = 6;
    const int buzzerPin = 7;
    const int tempAnalogPin = A1;
};

#endif // SENSOR_SYSTEM_H
