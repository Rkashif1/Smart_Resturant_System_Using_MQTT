#ifndef SENSOR_SYSTEM_H
#define SENSOR_SYSTEM_H

#include <WiFiNINA.h>
#include <PubSubClient.h>

class SensorSystem {
public:
    // Constructor
    SensorSystem(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttUser, const char* mqttPassword);
    
    // Initialize the system
    void begin();
    
    // Main loop to run the system
    void loop();

private:
    // Connect to Wi-Fi network
    void connectWiFi();
    
    // Reconnect to MQTT broker
    void reconnect();
    
    // Measure distance using ultrasonic sensor
    long measureDistance();
    
    // Read and publish sensor data
    void publishSensorData();

    // Wi-Fi and MQTT credentials
    const char* ssid;
    const char* password;
    const char* mqttServer;
    int mqttPort;
    const char* mqttUser;
    const char* mqttPassword;

    // Wi-Fi and MQTT clients
    WiFiClient wifiClient;
    PubSubClient client;

    // Pin definitions for sensors and actuators
    const int flameAnalogPin = A0;
    const int photoresistorPin = A5;
    const int trigPin = 9;
    const int echoPin = 8;
    const int greenLedPin = 11;
    const int redLedPin = 12;
    const int relayPin = 6;
    const int buzzerPin = 7;
    const int tempAnalogPin = A1;

    // Threshold values
    const int DISTANCE_THRESHOLD = 10;
};

#endif // SENSOR_SYSTEM_H
