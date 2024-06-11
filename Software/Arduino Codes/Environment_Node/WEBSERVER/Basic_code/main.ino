#include <WiFiNINA.h>

// WiFi credentials
const char* ssid = "FRITZ!Box 7510 XW";
const char* password = "46018957035297244418";

// Create a WiFi server object
WiFiServer server(80);

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
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

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
            String warningMessage = "";
            if (flameAnalogValue > 500) {
              buzzerState = true;
              warningMessage = "<p style='color:red;'>DANGER: High Flame Detected!</p>";
            } else if (temperature > 75) {
              buzzerState = true;
              warningMessage = "<p style='color:orange;'>WARNING: High Temperature!</p>";
            }

            // Update buzzer state and print debug info
            digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);
            Serial.print("Buzzer State: ");
            Serial.println(buzzerState ? "ON" : "OFF");
            Serial.print("Flame Analog Value: ");
            Serial.println(flameAnalogValue);
            Serial.print("Temperature: ");
            Serial.println(temperature);

            // Display the HTML web page with auto-refresh
            client.println("<html>");
            client.println("<head>");
            client.println("<title>Sensor Data</title>");
            client.println("<meta http-equiv=\"refresh\" content=\"0.5\">"); // Refresh every 0.5 seconds
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>Sensor Data</h1>");
            client.println("<p>Flame Sensor Analog: " + String(flameAnalogValue) + "</p>");
            client.println("<p>Photoresistor: " + String(lightLevel) + "</p>");
            client.println("<p>Temperature: " + String(temperature) + " &deg;C</p>");
            client.println("<p>Table Status: " + tableStatus + "</p>");
            client.println(warningMessage);
            client.println("</body>");
            client.println("</html>");

            // The HTTP response ends with another blank line
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Close the connection
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
