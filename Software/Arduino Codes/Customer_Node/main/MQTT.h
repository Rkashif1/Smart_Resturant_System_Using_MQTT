#ifndef MQTT_h
#define MQTT_h


#include "Arduino.h"
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

int count = 0;


void SendMessage(int Topicval, const char* topic) {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker

  const long interval = 1000;
  unsigned long previousMillis = 0;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(Topicval);

    mqttClient.beginMessage(topic);
    mqttClient.print(Topicval);
    mqttClient.endMessage();


    Serial.println();
  }
}

void SendMessageString(const char* Topicval, const char* topic) {
  const long interval = 1000;
  unsigned long previousMillis = 0;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(Topicval);

    mqttClient.beginMessage(topic);
    mqttClient.print(Topicval);
    mqttClient.endMessage();

    Serial.println();
  }
}


void EstablishConnection(char* ssid, char* pass, char* broker, int port)
{
    // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(2000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

}


#endif