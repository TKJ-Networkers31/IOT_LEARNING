#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
void smrtWifi();
void tmplIdnty();
void connectWifi();
const IPAddress routeIp(192,168,0,1);
void setup() {
  Serial.begin(9600);
  connectWifi();
  if(Ping.ping(routeIp)){
    Serial.println("Terhubung ke router");
  }
}

void loop() {
  Serial.println(Ping.averageTime());
  delay(2000);
}

void smrtWifi(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
  Serial.print("menunggu koneksi");
  while (!WiFi.smartConfigDone()){
    Serial.print("-");
    delay(500);
  }
  Serial.println("Menghubungkan..");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("WIFI CONNECTED");
  tmplIdnty();
}
void tmplIdnty(){
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());
} 
void connectWifi(){
  const String ssid = "iot";
  const String pw = "12345678";
  WiFi.begin(ssid,pw);
  Serial.println("Menghubungkan..");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("WIFI CONNECTED");
  tmplIdnty();
}