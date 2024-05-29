#include "MQTT.h"
#include "Sensor.h"

char ssid[] = "Crimson";        // your network SSID (name)
char pass[] = "JoJopart5";    // your network password (use for WPA, or use as key for WEP)


const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "LED_STATUS";


void setup() {

  Serial.begin(9600);

  EstablishConnection(ssid, pass, broker, port); // ssid, pass, broker, port

}

void loop() {
  
  SendMessage(Topicval, topic); // int Topicval, topic, Topicval is of type int, sends that numerical value.

}
