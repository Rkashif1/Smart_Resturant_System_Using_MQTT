import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient
import json

# MQTT settings
MQTT_SERVER = "mqtt.example.com"
MQTT_PORT = 1883
MQTT_USER = "yourMQTTUser"
MQTT_PASSWORD = "yourMQTTPassword"
MQTT_TOPICS = [("sensor/temperature", 0), ("sensor/humidity", 0), ("sensor/pressure", 0)]

# InfluxDB settings
INFLUXDB_SERVER = "localhost"
INFLUXDB_PORT = 8086
INFLUXDB_USER = "yourInfluxDBUser"
INFLUXDB_PASSWORD = "yourInfluxDBPassword"
INFLUXDB_DATABASE = "sensor_data"

# Initialize InfluxDB client
influx_client = InfluxDBClient(
    host=INFLUXDB_SERVER,
    port=INFLUXDB_PORT,
    username=INFLUXDB_USER,
    password=INFLUXDB_PASSWORD,
    database=INFLUXDB_DATABASE
)

# The callback for when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    for topic, qos in MQTT_TOPICS:
        client.subscribe(topic, qos)

# The callback for when a PUBLISH message is received from the server
def on_message(client, userdata, msg):
    print(f"Message received: {msg.topic} {msg.payload}")
    payload = msg.payload.decode("utf-8")
    
    # Prepare data for InfluxDB
    json_body = [
        {
            "measurement": msg.topic,
            "fields": {
                "value": float(payload)
            }
        }
    ]
    
    # Write to InfluxDB
    influx_client.write_points(json_body)

# Create an MQTT client instance
client = mqtt.Client()

# Set MQTT username and password
client.username_pw_set(MQTT_USER, MQTT_PASSWORD)

# Assign event callbacks
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
client.connect(MQTT_SERVER, MQTT_PORT, 60)

# Blocking call that processes network traffic, dispatches callbacks and handles reconnecting
client.loop_forever()
