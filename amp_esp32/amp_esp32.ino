#include <PubSubClient.h>

#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "w";
const char* password = "qwerty233";

// Replace with your MQTT broker's details
const char* mqttServer = "192.168.0.11";
const int mqttPort = 1883;
const char* mqttUser = "amp_esp32";  // If applicable
const char* mqttPassword = "password";  // If applicable

WiFiClient espClient;
PubSubClient client(espClient);

void setupWifi() {
    delay(10);
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
    }
    Serial.println();
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
            // Subscribe to a topic
            client.subscribe("test/topic");
        } else {
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setupWifi();
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Publish a message approximately every minute
    static unsigned long lastTime = 0;
    if (millis() - lastTime > 6000) {
        lastTime = millis();
        client.publish("test/topic", "Hello from ESP32");
    }
}
