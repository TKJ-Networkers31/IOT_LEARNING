#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

const IPAddress routerIP(192,168,0,1);

// String ssid = "haha"; //ssid wifi yang di konek
// String pw = "hahahaha"; //password dari ssid wifi yang digunakan
void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("ESP32 Starting...");
}

void loop() {
  Serial.println("Loop is running");
  delay(1000);
}