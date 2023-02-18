//mac 94-B9-7E-AD-45-D4
#include <M5StickCPlus.h>
#include <esp_now.h>
#include <WiFi.h>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <wire.h>
#include "JoyC.h"

uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0xF9, 0x56, 0x94};

int Pa = 0;
int Pd = 0;
int Pp = 0;
int La = 0;
int Lp = 0;
int Ld = 0;

int Pa_copy = Pa;
int Pd_copy = Pd;
int Pp_copy = Pp;
int La_copy = La;
int Lp_copy = Lp;
int Ld_copy = Ld;

JoyC joyc;
using namespace std;
TFT_eSprite display = TFT_eSprite(&M5.Lcd);

void vypis(const char *text,int posx,int posy){
  display.setCursor(posx, posy);
  display.drawString(text, posx, posy);
  display.pushSprite(0, 0);
}


// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  std::string data(reinterpret_cast<const char*>(incomingData), len);
  Serial.println(data.c_str());
  vypis(data.c_str(),10,40);
}

void setup()
{
  // Init Serial Monitor
  Serial.begin(115200);
  M5.begin();
  vypis("Start",10,10);
  Wire.begin(0, 26);
  display.createSprite(300,100);
  display.fillSprite(TFT_BLACK);
  display.setTextColor(TFT_WHITE);
  display.setTextSize(2);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  for (int ii = 0; ii < 6; ++ii )
  {
    peerInfo.peer_addr[ii] = (uint8_t) broadcastAddress[ii];
  }
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

}

void loop()
{
  //1 = pravá
  //2 = levá
  Pa = joyc.GetAngle(1);
  Pd = joyc.GetDistance(1);
  Pp = joyc.GetPress(1);
  La = joyc.GetAngle(0);
  Ld = joyc.GetDistance(0);
  Lp = joyc.GetPress(0);
  // Send a message
  
  if (Pa != Pa_copy || Pd != Pd_copy || Pp != Pp_copy || La != La_copy || Lp != Lp_copy || Ld != Ld_copy) {
    std::string hodnoty = std::to_string(Pa) + " " + std::to_string(Pd) + " " + std::to_string(Pp) + " " + std::to_string(La) + " " + std::to_string(Lp) + " " + std::to_string(Ld);
    const char* outgoingData = hodnoty.c_str();
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*) outgoingData, strlen(outgoingData) + 1);
    if (result == ESP_OK) {
      Serial.println("ok");
    }
    Pa_copy = Pa;
    Pd_copy = Pd;
    Pp_copy = Pp;
    La_copy = La;
    Lp_copy = Lp;
    Ld_copy = Ld;
    display.fillSprite(TFT_BLACK);
    vypis("Povidame si",10,10);
  }
  //delay(100);
  // Wait for 5 seconds before sending the next message  
}