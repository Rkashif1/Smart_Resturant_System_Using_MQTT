# Smart Restaurant System Using MQTT

This project presents a **Smart Restaurant System Using MQTT** that leverages IoT and MQTT protocols to enhance restaurant management and customer experience. Using a Raspberry Pi as the MQTT broker and Arduino-based sensor nodes, the system monitors environmental conditions and customer interactions, displaying real-time data with InfluxDB and Grafana.

## Project Overview

The Smart Restaurant System Using MQTT integrates IoT technology to streamline restaurant operations. Various sensor nodes, powered by Arduino, collect data on factors such as ambient conditions and customer interactions, relaying this information to a central Raspberry Pi broker via MQTT. Data is stored and visualized in real-time, providing insights that help improve restaurant efficiency and customer satisfaction.

## Features

- **MQTT Protocol for Data Transmission:** The MQTT protocol enables efficient data transfer between sensor nodes and the central broker, ensuring timely updates.
- **Environmental Monitoring:** Sensors collect data on conditions such as temperature, humidity, and air quality to maintain an optimal dining environment.
- **Customer Interaction Tracking:** Monitors customer activities and preferences to enhance service quality.
- **Data Visualization with InfluxDB and Grafana:** Real-time data visualization provides actionable insights for restaurant management.

## Hardware Components

- **Raspberry Pi:** Acts as the central MQTT broker and data storage device.
- **Arduino Sensor Nodes:** Equipped with sensors to monitor environmental data and customer interactions.
- **Sensors:** Various sensors to capture data on temperature, humidity, and other parameters.
- **InfluxDB and Grafana:** Used for data storage and visualization, allowing users to monitor metrics in real-time.

## Setup and Usage

1. **Hardware Setup:** Connect the Arduino sensor nodes to various sensors and link them to the Raspberry Pi.
2. **MQTT Broker Configuration:** Configure the Raspberry Pi as an MQTT broker to handle data communication.
3. **Data Storage:** Set up InfluxDB on the Raspberry Pi to collect data from sensor nodes.
4. **Visualization:** Use Grafana to visualize real-time data and monitor restaurant conditions.
5. **Testing:** Test sensor data transmission and visualization to ensure the system is fully operational.

## Future Enhancements

- Add predictive analytics for enhanced operational efficiency.
- Integrate machine learning to analyze customer preferences.
- Develop a mobile app interface for managers to monitor restaurant conditions remotely.

## Team Collaboration

Each team member contributed to designing, implementing, and testing the Smart Restaurant System. Collaborative efforts ensured a robust solution that meets the demands of modern restaurant management.

## Conclusion

The Smart Restaurant System Using MQTT is an innovative solution that brings IoT and data visualization to restaurant management. By monitoring environmental conditions and customer interactions, it helps create a smarter, more responsive dining environment that benefits both customers and management.
