#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <PubSubClient.h>

void smrtWifi();
void connWifi();
void prntWifi();
void connMqtt();
void recmass(char* topic, byte* isi, unsigned int length);

const String google = "www.google.com";
const String dns = "broker.emqx.io";
const IPAddress server(10,141,113,98);
WiFiClient client;
PubSubClient mqtt(client);
int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  connWifi();
  mqtt.setServer(server,1883);
}

void loop() {
  if(!mqtt.connected()){
    connMqtt();
    mqtt.publish("esp32/coba", "esp32 connected");
  }
  mqtt.loop();
  mqtt.setCallback(recmass);
}

void smrtWifi(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
  Serial.println("mencari koneksi...");
  while (!WiFi.smartConfigDone()){
    Serial.print("-");
    delay(500);
  }
  Serial.println("menghubungkan...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
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
    delay(500);
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
      Serial.println("berahasil connect yhuuuuuu");
    }
  }
}

void recmass(char* topic, byte* isi, unsigned int length){
  String perintah = "";
  Serial.print("persan diterima dari topik : ");
  Serial.println(topic);
  Serial.print("isi pesan : ");
  for (int i = 0; i < length; i++){
    Serial.print(char(isi[i]));
    perintah += char(isi[i]);
  }
  Serial.println();
  if (perintah == "ON"){
    digitalWrite(led, HIGH);
  }else if (perintah == "OFF"){
    digitalWrite(led, LOW);
  }else{
    // mqtt.publish("esp32/coba", "command salah");
  }  
}

