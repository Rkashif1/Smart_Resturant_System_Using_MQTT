#include "SensorSystem.h"

SensorSystem::SensorSystem(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttUser, const char* mqttPassword)
  : ssid(ssid), password(password), mqttServer(mqttServer), mqttPort(mqttPort), mqttUser(mqttUser), mqttPassword(mqttPassword), client(wifiClient) {}

void SensorSystem::begin() {
    Serial.begin(115200);

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

void SensorSystem::loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    publishSensorData();
    delay(1000); // Adjust delay as needed
}

void SensorSystem::connectWiFi() {
    Serial.print("Connecting to WiFi...");
    while (WiFi.begin(ssid, password) != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void SensorSystem::reconnect() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect("ArduinoClient", mqttUser, mqttPassword)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

long SensorSystem::measureDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2;
    return distance;
}

void SensorSystem::publishSensorData() {
    long distance = measureDistance();
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

    int flameAnalogValue = analogRead(flameAnalogPin);
    int lightLevel = analogRead(photoresistorPin);
    int tempAnalogValue = analogRead(tempAnalogPin);
    float voltage = tempAnalogValue * (5.0 / 1023.0);
    float temperature = voltage * 100.0;

    if (lightLevel < 200) {
        digitalWrite(relayPin, LOW);
    } else {
        digitalWrite(relayPin, HIGH);
    }

    bool buzzerState = false;
    if (flameAnalogValue > 500 || temperature > 75) {
        buzzerState = true;
    }
    digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);

    client.publish("sensor/flame", String(flameAnalogValue).c_str());
    client.publish("sensor/light", String(lightLevel).c_str());
    client.publish("sensor/temperature", String(temperature).c_str());
    client.publish("sensor/tableStatus", tableStatus.c_str());
}
