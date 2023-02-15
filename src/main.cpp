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

int cPa, cPd, cPp, cLa, cLd, cLp;
int lPa, lPd, lPp, lLa, lLd, lLp;

JoyC joyc;

TFT_eSprite display = TFT_eSprite(&M5.Lcd);

void vypis(const char *text){
  display.fillSprite(TFT_BLACK);
  display.setCursor(10, 10);
  display.drawString(text, 0, 0);
  display.pushSprite(0, 0);
}

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
    vypis("Pripojuji");
  }

}

void loop() {
  WiFiClient client;

  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("nevim kde je server");
    vypis("Nelze pripojit");
    Serial.println(WiFi.localIP());
    delay(1000);
    return;
  }
  vypis("Povidam si");
  //1 = pravý 
  //0 = levý
  delay(500);
  cPa = joyc.GetAngle(1);
  cPd = joyc.GetDistance(1);
  cPp = joyc.GetPress(1);
  cLa = joyc.GetAngle(0);
  cLd = joyc.GetDistance(0);
  cLp = joyc.GetPress(0);

  if (cPa != lPa || cPd != lPd || cPp != lPp || cLa != lLa || cLd != lLd || cLp != lLp){
    std::string zprava = std::to_string(cPa) + " " + std::to_string(cPd) + " " + std::to_string(cPp) + " " + std::to_string(cLa) + " " + std::to_string(cLd) + " " + std::to_string(cLp);
    String z = String(zprava.c_str());
    client.print(z);
    client.stop();
    Serial.println("odeslano");
    lPa = cPa;
    lPd = cPd;
    lPp = cPp;
    lLa = cLa;
    lLd = cLd;
    lLp = cLp;
  }
}
