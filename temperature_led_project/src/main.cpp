// //library
#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// //deklarasi fungsi
void smrtWifi();
void connWifi();
void spekWifi();
void connbroker();
void reciveM(char* topic, byte* payload, unsigned int length);

// //deklarasi variabel
const IPAddress gateway(192,168,0,1);
const IPAddress serverBroker(192,168,1,2);
const String google = "www.google.com";
WiFiClient client;
PubSubClient mqtt(client);

void setup() {
  Serial.begin(9600);
  dht.begin();
  connWifi();
  mqtt.setServer(serverBroker,1883);
  connbroker();
  mqtt.setKeepAlive(60);
  mqtt.setCallback(reciveM);
}

void loop() {
  while (WiFi.status() != WL_CONNECTED){
    Serial.println("connection lost");
    Serial.println("reconecting..");
    connWifi();
  }
  while (!mqtt.connected())
  {
    Serial.println("MQTT disconnected... reconnecting...");
    connbroker();
  }
  mqtt.loop();
  float suhu = dht.readTemperature();    // Baca suhu (Celsius)
  float lembab = dht.readHumidity(); 
  
  // Konversi data ke string
  char suhuStr[8];
  char lembabStr[8];
  dtostrf(suhu, 1, 2, suhuStr);
  dtostrf(lembab, 1, 2, lembabStr);  
  mqtt.publish("esp/suhu", suhuStr);
  mqtt.publish("esp/kelembapan", lembabStr);

  Serial.printf("Kirim MQTT → Suhu: %.2f °C | Kelembapan: %.2f %%\n", suhu, lembab);

  delay(3000);
}

void smrtWifi(){
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
  Serial.println("menunggu koneksi eksternal----");
  while (!WiFi.smartConfigDone()){
    Serial.print("-");
    delay(500);
  }
  Serial.print("connecting to wifi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("wifi connected");
  if(Ping.ping(gateway)){
    Serial.println("terhubung ke gateway");
    if(Ping.ping(google.c_str())){
      Serial.print("terhubung ke internet");
    }else{
      Serial.println("tidak terhubung ke internet");
    }
  }else{
    Serial.println("cek router gateway");
  }
  spekWifi();
  
  
}
void connWifi(){
  const String ssid = "iot";
  const String password = "12345678";
  WiFi.begin(ssid.c_str(),password.c_str());
  Serial.print("connecting to wifi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("wifi connected");
  if(Ping.ping(gateway)){
    Serial.println("terhubung ke gateway");
    if(Ping.ping(google.c_str())){
      Serial.print("terhubung ke internet");
    }else{
      Serial.println("tidak terhubung ke internet");
    }
  }else{
    Serial.println("cek router gateway");
  }
  spekWifi();
}
void spekWifi(){
  Serial.println("DATA WIFI TERKONEKSI");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());
}
void connbroker(){
  Serial.println("connecting to broker....");
  while (!mqtt.connected()){
    if(mqtt.connect("esp32-1")){
      mqtt.subscribe("iot/suhu");
      // mqtt.subscribe("esp/control");
      Serial.println("connected to broker");
      }else{
      Serial.print("gagal koneksi, rc= ");
      switch (int(mqtt.state())){
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

void reciveM(char* topic, byte* payload, unsigned int length){
  String massage = "";
  for (int i = 0; i < length; i++){
    massage += char(payload[i]);
  }
}