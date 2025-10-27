//library
#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <PubSubClient.h>

//deklarasi fungsi
void conectWifi();
void smrtConnect();
void tampilId();
void conectbroker();
void receiveM(char* topic, byte* isi, unsigned int length);

//deklarasi variabel
const int led = 13;
const IPAddress serverBroker(192,168,0,2);
const IPAddress gateway(192,168,0,2);
const String google = "www.google.com";
WiFiClient client;
PubSubClient mqtt(client);

//fungsi utama
void setup() {
  Serial.begin(9600);
  conectWifi();
  mqtt.setServer(serverBroker,1883);
  mqtt.setKeepAlive(60);
  conectbroker();
  mqtt.setCallback(receiveM);
}

void loop() {
  while (WiFi.status() != WL_CONNECTED){
    Serial.println("connection lost");
    Serial.println("reconecting..");
    conectWifi();
  }
  while (!mqtt.connected())
  {
    Serial.println("MQTT disconnected... reconnecting...");
    conectbroker();
  }
  mqtt.loop();
  String suhu = String(random(25, 32));
  mqtt.publish("iot/suhu", suhu.c_str(), true);
  Serial.println("Mengirim suhu: " + suhu);
  delay(5000);
}
void conectWifi(){
  const String ssid = "iot";
  const String password = "12345678";
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("connecting...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  tampilId();
  Serial.println("");
  if (Ping.ping(gateway)){
    Serial.println("terhubung ke gateway...");
    if (Ping.ping(google.c_str())){
      Serial.println("terhubung ke internet");
    }else{
      Serial.println("tidak terhubung ke internet");
    }
  }
  Serial.println("");
}
void smrtConnect(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
  Serial.print("menunggu koneksi---");
  while(!WiFi.smartConfigDone()){
    Serial.print("-");
    delay(500);
  }
  Serial.print("connecting...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  if (Ping.ping(gateway)){
    Serial.println("terhubung ke gateway...");
    if (Ping.ping(google.c_str())){
      Serial.println("terhubung ke internet");
    }else{
      Serial.println("tidak terhubung ke internet");
    }
  }
  Serial.println("");
  tampilId();
}
void tampilId(){
  Serial.println("STATUS WIFI");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());
}
void conectbroker(){
  Serial.println("connecting to mqtt");
  while (!mqtt.connected()){
    if(mqtt.connect("esp-1")){
      mqtt.subscribe("iot/suhu");
      mqtt.subscribe("esp/control");
      Serial.println("connected to broker");
    }else{
      Serial.print("gagal koneksi, rc= ");
      switch (int(mqtt.state()))
      {
      case -4:
        Serial.println("Time out");
        break;
      case -2:
        Serial.println("Server not found");
        break;
      case -1:
        Serial.println("filed to connect");
        break;
      case 2:
        Serial.println("protocol error");
        break;
      case 4:
        Serial.println("username/password salah");
        break;
      default:
        break;
      }
      delay(2000);
    }
  }
  
}
void receiveM(char* topic, byte* isi, unsigned int length){
  String massage = "";
  for (int i = 0; i < length; i++){
    Serial.print(char(isi[i]));
    massage += char(isi[i]);
  }
  if(String(topic) == "esp/control"){
    Serial.println("receive message by topic : ");
    Serial.print(topic);
    if (massage == "ON"){
      digitalWrite(led, HIGH);
    }else if (massage == "OFF"){
      digitalWrite(led,LOW);
    }else{

    }
  }
  Serial.println();
}
