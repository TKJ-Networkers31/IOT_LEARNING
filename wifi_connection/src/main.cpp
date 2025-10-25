#include <Arduino.h>
#include <WiFi.h>

String ssid = "hah"; //ssid wifi yang di konek
String pw = "hahahaha"; //password dari ssid wifi yang digunakan
void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid.c_str(), pw.c_str()); //connect ke wifi
  Serial.begin(9600); //memulai komunikasi UART dengan kecepatan transfer 9600bit
  Serial.println("connecting..");
  while (WiFi.status() != WL_CONNECTED){//cek apakah status wifi tidak konek?
    Serial.print(".");//tampilkan .
    delay(500);
  }
  Serial.println("wifi connected");
}

void loop() {
  // put your main code here, to run repeatedly:
}
