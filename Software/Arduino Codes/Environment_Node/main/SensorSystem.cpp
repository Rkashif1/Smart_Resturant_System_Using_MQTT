#include "SensorSystem.h"

// Constructor for SensorSystem, initializes member variables
SensorSystem::SensorSystem(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttUser, const char* mqttPassword)
  : ssid(ssid), password(password), mqttServer(mqttServer), mqttPort(mqttPort), mqttUser(mqttUser), mqttPassword(mqttPassword), client(wifiClient) {}

void SensorSystem::begin() {
    Serial.begin(115200); // Initialize serial communication

    // Initialize pins for sensors and actuators
    pinMode(flameAnalogPin, INPUT);
    pinMode(photoresistorPin, INPUT);
    pinMode(relayPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(tempAnalogPin, INPUT);

    connectWiFi(); // Connect to Wi-Fi
    client.setServer(mqttServer, mqttPort); // Set MQTT server and port
}

void SensorSystem::loop() {
    if (!client.connected()) {
        reconnect(); // Reconnect to MQTT if connection is lost
    }
    client.loop(); // Maintain MQTT connection
    publishSensorData(); // Read and publish sensor data
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
    Serial.println(WiFi.localIP()); // Print the IP address once connected
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
    return duration * 0.034 / 2; // Calculate distance in cm
}

void SensorSystem::publishSensorData() {
    long distance = measureDistance(); // Measure distance using ultrasonic sensor
    String tableStatus;
    if (distance < DISTANCE_THRESHOLD) {
        tableStatus = "Reserved";
        digitalWrite(greenLedPin, LOW);
        digitalWrite(redLedPin, HIGH); // Turn on red LED if table is reserved
    } else {
        tableStatus = "Available";
        digitalWrite(greenLedPin, HIGH); // Turn on green LED if table is available
        digitalWrite(redLedPin, LOW);
    }

    // Read sensor values
    int flameAnalogValue = analogRead(flameAnalogPin);
    int lightLevel = analogRead(photoresistorPin);
    int tempAnalogValue = analogRead(tempAnalogPin);
    float voltage = tempAnalogValue * (5.0 / 1023.0);
    float temperature = voltage * 100.0;

    // Control relay based on light level
    if (lightLevel < 200) {
        digitalWrite(relayPin, LOW); // Turn on light
    } else {
        digitalWrite(relayPin, HIGH); // Turn off light
    }

    // Control buzzer based on flame or temperature threshold
    bool buzzerState = false;
    if (flameAnalogValue > 500 || temperature > 75) {
        buzzerState = true;
    }
    digitalWrite(buzzerPin, buzzerState ? HIGH : LOW); // Activate buzzer if threshold exceeded

    // Publish sensor values to MQTT topics
    client.publish("sensor/flame", String(flameAnalogValue).c_str());
    client.publish("sensor/light", String(lightLevel).c_str());
    client.publish("sensor/temperature", String(temperature).c_str());
    client.publish("sensor/tableStatus", tableStatus.c_str());
}
