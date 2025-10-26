#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

const IPAddress routerIP(192,168,0,1);

// String ssid = "haha"; //ssid wifi yang di konek
// String pw = "hahahaha"; //password dari ssid wifi yang digunakan
void setup() {
  // WiFi.begin(ssid.c_str(), pw.c_str()); //connect ke wifi
  WiFi.mode(WIFI_AP_STA);// uba mode wifi 
  Serial.begin(9600); //memulai komunikasi UART dengan kecepatan transfer 9600bit
  // Serial.println("connecting..");
  // while (WiFi.status() != WL_CONNECTED){//cek apakah status wifi tidak konek?
  //   Serial.print(".");//tampilkan .
  //   delay(500);
  // }
  WiFi.beginSmartConfig(); // jalankan wifi
  while (!WiFi.smartConfigDone()){//cek apakah status wifi sudah konek?
    Serial.print("-");//tampilkan - jika belum terkoneksi
    delay(500);
  }
  while (WiFi.status() != WL_CONNECTED){//cek apakah status wifi tidak konek?
    Serial.print(".");//tampilkan .
    delay(500);
  }
  Serial.println("wifi connected");
  if(Ping.ping(routerIP)){ //ping ke ip router 
    Serial.println("esp connected to router"); //jika true tampilkan ini
  }
  Serial.println(WiFi.SSID()); //tampilkan nama wifi
  Serial.println(WiFi.RSSI()); //tampilkan kualitas wifi satuan dBm
  Serial.println(WiFi.macAddress()); //tampilkan mac address
  Serial.println(WiFi.localIP()); //tampilkan ip address
  Serial.println(WiFi.gatewayIP()); //tampilkan gateway
  Serial.println(WiFi.dnsIP()); //tampilkan dns server
}

void loop() {
  Serial.println(Ping.averageTime());//tampilkan rata rata ping ke ip tujuan
  delay(2000); //delay selama 2 detik
}
