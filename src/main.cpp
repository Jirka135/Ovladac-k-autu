#include <WiFi.h>
#include <M5StickCPlus.h>
#include <iostream>
#include <string>
#include <Wire.h>
#include "JoyC.h"

const char* ssid = "jirka";
const char* password = "jirkajebest";

const char* serverAddress = "192.168.0.110"; // Replace with the IP address of the server ESP32
const int serverPort = 80;

JoyC joyc;

TFT_eSprite display = TFT_eSprite(&M5.Lcd);

void setup() {
  Serial.begin(115200);
  M5.begin();
  display.createSprite(135, 240);
  display.fillSprite(TFT_BLACK);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextSize(2);
  Wire.begin(0, 26, 400000UL);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("připojuji se k WiFi...");
    display.setCursor(10, 10);
    display.drawString("Pripojuji", 0, 0);
    display.pushSprite(0, 0);
  }
  Serial.println("Pripojeno");
  display.setCursor(10, 10);
  display.drawString("Pripojeno", 0, 0);
  display.pushSprite(0, 0);

}

void loop() {
  WiFiClient client;
  display.fillSprite(TFT_BLACK);
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("nevim kde je server");
    display.setCursor(10, 10);
    display.drawString("nelze pripojit", 0, 0);
    display.pushSprite(0, 0);
    delay(1000);
    return;
  }else{
    display.setCursor(10, 10);
    display.drawString("pripojeno", 0, 0);
    display.pushSprite(0, 0);
  }


  //String response = client.readStringUntil('\r');
  //Serial.println("Response from server: " + response);

  char text_buff[100];

  //1 = pravý 
  //0 = levý
  int Pa,Pd,Pp,La,Ld,Lp;
  Pa = joyc.GetAngle(1);
  Pd = joyc.GetDistance(1);
  Pp = joyc.GetPress(1);
  
  La = joyc.GetAngle(0);
  Ld = joyc.GetDistance(0);
  Lp = joyc.GetPress(0);
  

  std::string zprava = std::to_string(Pa)+" "+ std::to_string(Pd)+" "+ std::to_string(Pp)+" "+ std::to_string(Ld)+" "+ std::to_string(Ld)+" "+ std::to_string(Ld);
  String z = String(zprava.c_str());
  client.print(z); 
  Serial.println("odeslano");
  delay(100);
  client.stop();


}