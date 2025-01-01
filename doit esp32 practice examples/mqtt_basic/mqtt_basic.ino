#include <WiFi.h>
#include <PubSubClient.h>
#include<iostream>
#include<string>

#define TRIG_PIN 13
#define ECHO_PIN 14

// Blue LED Pin (optional)
#define BLUE_LED 2

// WiFi Credentials
#define WIFI_SSID "Scutum"
#define WIFI_PASSWORD "00000111"

// MQTT Broker Details
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_TOPIC "seat/1/status" // Topic to send seat status

// WiFi and MQTT Client Setup
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(BLUE_LED,OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  //keep connection alive for 1000 milli seconds
  client.setKeepAlive(30);  // MQTT Keep-alive time
  // Connect to MQTT Broker
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

unsigned long lastUpdate = 0;
const int interval = 1000; // 1 second

void loop() {
   // Check if WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    reconnectWiFi();
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();  // Maintain connection

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= interval) {
    lastUpdate = currentMillis;
    float distance = measureDistance();  // Distance function
    Serial.println(distance);
    
    // Use String class for Arduino, ya std:: string sa alag ha jho c++ may hota ha, String is arduino core class
    String status = String(distance);   // Convert float to String
    
    Serial.println(client.state());
    
    // Use status as needed
    publishSeatStatus(status);
    
  }
}

// Function to reconnect to WiFi if disconnected
void reconnectWiFi() {
  Serial.println("WiFi disconnected. Attempting to reconnect...");

  // Attempt to reconnect until successful
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);  // Wait for 5 seconds before trying again
  }

  // Once connected, print the IP address
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void publishSeatStatus(String status) {
  client.publish(MQTT_TOPIC, status.c_str()); // Send "ON" or "OFF"
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32Client_" + String(ESP.getEfuseMac()); 
    client.connect(clientId.c_str());
    if (client.connected()) {
      Serial.println("Connected to MQTT Broker");
    } else {
      delay(5000);
    }
  }
}

float measureDistance() {
  const int samples = 1; // taking multiple samples messing with delays and 7 segment multiplexing, so keep it to 1 only.
  float total = 0;

  for (int i = 0; i < samples; i++) {
    // Trigger ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure echo duration
    long duration = pulseIn(ECHO_PIN, HIGH);
    total += duration * 0.0343 / 2; // Convert to cm
  }

  return total / samples; // Return average distance
}
