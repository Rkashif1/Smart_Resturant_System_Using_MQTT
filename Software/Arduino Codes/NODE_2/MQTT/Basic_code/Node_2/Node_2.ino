#include <WiFiNINA.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "FRITZ!Box 7510 XW";
const char* password = "46018957035297244418";

// MQTT broker details
const char* mqttServer = "mqtt.example.com"; // Replace with your MQTT broker address
const int mqttPort = 1883;
const char* mqttUser = "yourMQTTUser"; // Replace with your MQTT username
const char* mqttPassword = "yourMQTTPassword"; // Replace with your MQTT password

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Sensor pins
const int flameAnalogPin = A0;
const int photoresistorPin = A5;
const int trigPin = 9; // Ultrasonic trig pin
const int echoPin = 8; // Ultrasonic echo pin
const int greenLedPin = 11; // Green LED pin
const int redLedPin = 12; // Red LED pin
const int relayPin = 6; // Relay connected to digital pin 6
const int buzzerPin = 7; // Piezo buzzer connected to digital pin 7
const int tempAnalogPin = A1; // Analog temperature sensor connected to A1

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ArduinoClient", mqttUser, mqttPassword)) {
      Serial.println("connected");
      // Subscribe to topics if needed
      // client.subscribe("yourTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); 
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  Serial.begin(115200);

  // Initialize sensor pins
  pinMode(flameAnalogPin, INPUT);
  pinMode(photoresistorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT); 
  pinMode(tempAnalogPin, INPUT);

  connectWiFi();

  client.setServer(mqttServer, mqttPort);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Measure distance using ultrasonic sensor
  long distance = measureDistance();

  // Determine table status based on ultrasonic sensor
  String tableStatus;
  if (distance < 50) {
    tableStatus = "Reserved";
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
  } else {
    tableStatus = "Available";
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  }

  // Read sensor values
  int flameAnalogValue = analogRead(flameAnalogPin);
  int lightLevel = analogRead(photoresistorPin);
  int tempAnalogValue = analogRead(tempAnalogPin);

  // Convert the analog reading to temperature in Celsius
  float voltage = tempAnalogValue * (5.0 / 1023.0);
  float temperature = voltage * 100.0;

  // Control the relay based on the photoresistor value
  if (lightLevel < 200) { // Example threshold value
    digitalWrite(relayPin, LOW); // Turn relay ON (LED ON)
  } else {
    digitalWrite(relayPin, HIGH); // Turn relay OFF (LED OFF)
  }

  // Control the buzzer based on the flame sensor value and temperature
  bool buzzerState = false;
  if (flameAnalogValue > 500) {
    buzzerState = true;
  } else if (temperature > 75) {
    buzzerState = true;
  }

  // Update buzzer state and print debug info
  digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);
  Serial.print("Buzzer State: ");
  Serial.println(buzzerState ? "ON" : "OFF");
  Serial.print("Flame Analog Value: ");
  Serial.println(flameAnalogValue);
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Publish sensor data to MQTT topics
  client.publish("sensor/flame", String(flameAnalogValue).c_str());
  client.publish("sensor/light", String(lightLevel).c_str());
  client.publish("sensor/temperature", String(temperature).c_str());
  client.publish("sensor/tableStatus", tableStatus.c_str());

  delay(1000); // Adjust delay as needed
}
