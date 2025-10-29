#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

void connectWiFi();
void connectMQTT();
void receiveMQTT(char* topic, byte* payload, unsigned int length);
void sendSensorData();

// ----- KONFIGURASI -----
const char* ssid     = "iot";
const char* password = "12345678";
const char* mqttServer = "192.168.1.2";

const int ledPin = 13;
const int relayPin = 22;
#define DHTPIN 23
#define DHTTYPE DHT11

WiFiClient espClient;
PubSubClient mqtt(espClient);
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 5000; // 5 detik

// ----- SETUP -----
void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("ESP32 Ready!");

  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  dht.begin();

  connectWiFi();
  mqtt.setServer(mqttServer, 1883);
  mqtt.setCallback(receiveMQTT);
  connectMQTT();
}

// ----- LOOP -----
void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  
  mqtt.loop(); // biar callback jalan

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendSensorData();
  }
}

// ----- WIFI -----
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
}

// ----- MQTT -----
void connectMQTT() {
  Serial.print("Connecting to MQTT...");
  while (!mqtt.connected()) {
    if (mqtt.connect("esp32-1")) {
      Serial.println("connected!");
      mqtt.subscribe("esp/control"); // untuk perintah ON/OFF
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 2s");
      delay(2000);
    }
  }
}

// ----- RECEIVE MQTT -----
void receiveMQTT(char* topic, byte* payload, unsigned int length) {
  char msg[length + 1];
  memcpy(msg, payload, length);
  msg[length] = '\0';

  Serial.print("Received on [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);

  if (String(topic) == "esp/control") {
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, msg);
    if (error) {
      Serial.println("JSON parse failed!");
      return;
    }

    const char* relayCmd = doc["relay"];
    const char* ledCmd   = doc["led"];

    if (relayCmd) digitalWrite(relayPin, (String(relayCmd) == "ON") ? HIGH : LOW);
    if (ledCmd)   digitalWrite(ledPin,   (String(ledCmd) == "ON") ? HIGH : LOW);

    Serial.print("Relay: "); Serial.println(digitalRead(relayPin) ? "ON" : "OFF");
    Serial.print("LED: ");   Serial.println(digitalRead(ledPin) ? "ON" : "OFF");
  }
}

// ----- SEND JSON -----
void sendSensorData() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  StaticJsonDocument<128> doc;
  doc["device"] = "esp32_dapur";
  doc["temperature"] = temp;
  doc["humidity"] = hum;
  doc["relay"] = digitalRead(relayPin) ? "ON" : "OFF";
  doc["led"]   = digitalRead(ledPin) ? "ON" : "OFF";

  char buffer[128];
  size_t n = serializeJson(doc, buffer);
  mqtt.publish("esp/lab1", buffer, n);

  Serial.print("Sent: ");
  Serial.println(buffer);
}
