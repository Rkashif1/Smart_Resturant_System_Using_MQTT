#include "MQTT.h"
#include "Sensor.h"

char ssid[] = "Vodafone-2ED3";        // your network SSID (name)
char pass[] = "PHWukHCXpe3ifdXV";    // your network password (use for WPA, or use as key for WEP)


const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "LED_STATUS";
uint8_t State = 0;
uint8_t Topicval = 0;


void setup() {

  Serial.begin(9600);

  EstablishConnection(ssid, pass, broker, port); // ssid, pass, broker, port

}

void loop() {

  mqttClient.poll();

  //CustomerPing(State);

  Topicval = 325;
  
  SendMessage(Topicval, topic); // int Topicval, topic, Topicval is of type int, sends that numerical value.

}
