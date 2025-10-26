#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <PubSubClient.h>

void smrtWifi();
void connWifi();
void prntWifi();
void connMqtt();

const String google = "www.google.com";
const String dns = "broker.emqx.io";
const IPAddress server(192,168,0,2);
WiFiClient client;
PubSubClient mqtt(client);

void setup() {
  Serial.begin(9600);
  connWifi();
  mqtt.setServer(dns.c_str(),1883);
}

void loop() {
  if(!mqtt.connected()){
    connMqtt();
  }
  mqtt.loop();
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

void connMqtt(){
  Serial.println("connecting to mqtt....");
  while (!mqtt.connected()){
    if(mqtt.connect("esp32-1")){
      mqtt.subscribe("esp32/coba");
    }
  }
  
}

