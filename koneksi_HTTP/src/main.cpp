#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32Ping.h>
void smrtWifi();
void tmplIdnty();
void connectWifi();
void getHttp();
void posthttp();
const IPAddress routeIp(192,168,0,1);
const String google = "www.google.com";
void setup() {
  Serial.begin(9600);
  connectWifi();
  if(Ping.ping(google.c_str())){
    Serial.println("Terhubung ke internet");
  }
  getHttp();
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

void getHttp(){
  String url = "https://reqres.in/api/users/2";
  HTTPClient http;
  String respon;
  http.begin(url);
  http.GET();
  respon = http.getString();
  Serial.println(respon);
  JsonDocument doc;
  deserializeJson(doc,respon);
  JsonObject obj = doc.as<JsonObject>();
  String data = obj[String("data")];
  Serial.println(data);
}

void postHttp(){
  
}