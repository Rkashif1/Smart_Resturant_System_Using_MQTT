#include <WiFiNINA.h>
#include "Node_2.h"

void setup() {
  Serial.begin(115200);
  initializePins();
  connectWiFi();
  server.begin();
}

void loop() {
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
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

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
            String warningMessage = "";
            if (flameAnalogValue > 500) {
              buzzerState = true;
              warningMessage = "<p style='color:red;'>DANGER:  Flame Detected!</p>";
            } else if (temperature > 70) {
              buzzerState = true;
              warningMessage = "<p style='color:orange;'>WARNING: High Temperature!</p>";
            }

            digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);

            client.println("<html>");
            client.println("<head>");
            client.println("<title>Sensor Data</title>");
            client.println("<meta http-equiv=\"refresh\" content=\"0.5\">");
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
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
