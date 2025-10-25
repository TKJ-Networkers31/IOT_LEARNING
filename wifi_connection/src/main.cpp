#include <Arduino.h>
#include <WiFi.h>

// String ssid = "haha"; //ssid wifi yang di konek
// String pw = "hahahaha"; //password dari ssid wifi yang digunakan
void setup() {
  // put your setup code here, to run once:
  // WiFi.begin(ssid.c_str(), pw.c_str()); //connect ke wifi
  Serial.begin(9600); //memulai komunikasi UART dengan kecepatan transfer 9600bit
  // Serial.println("connecting..");
  // while (WiFi.status() != WL_CONNECTED){//cek apakah status wifi tidak konek?
  //   Serial.print(".");//tampilkan .
  //   delay(500);
  // }
  WiFi.mode(WIFI_AP_STA);// uba mode wifi 
  WiFi.beginSmartConfig(); // jalankan wifi
  while (!WiFi.smartConfigDone()){//cek apakah status wifi sudah koneka?
    Serial.print("-");//tampilkan .
    delay(500);
  }
  while (WiFi.status() != WL_CONNECTED){//cek apakah status wifi tidak konek?
    Serial.print(".");//tampilkan .
    delay(500);
  }
  Serial.println("wifi connected");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());
}

void loop() {
  // put your main code here, to run repeatedly:
}
