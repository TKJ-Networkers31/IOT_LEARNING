#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

void smrtWifi();
void connWifi();
void prntWifi();

const String google = "www.google.com";

void setup() {

}

void loop() {

}

void smrtWifi(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
  Serial.println("mencari koneksi...");
  while (!WiFi.smartConfigDone()){
    Serial.print("-");
  }
  Serial.println("menghubungkan...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
  }
  Serial.println("connected");
  prntWifi();
  if(Ping.ping(google.c_str())){
    Serial.println("terhubung ke internet");
  }else{
    Serial.println("gagal terhubung");
  }
}

void connWifi(){
  String ssid = "haha";
  String pw = "hahahaha";
  WiFi.begin(ssid.c_str(),pw.c_str());
  Serial.println("menghubungkan...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
  }
  Serial.println("connected");
  prntWifi();
  if(Ping.ping(google.c_str())){
    Serial.println("terhubung ke internet");
  }else{
    Serial.println("gagal terhubung");
  }
}

void prntWifi(){
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());
  if(Ping.ping(google.c_str())){
    Serial.println("terhubung ke internet");
  }else{
    Serial.println("gagal terhubung");
  }
}

