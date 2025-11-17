#include <stdint.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>

#include "Transmit.h"
#include "Display.h"
#include "Globals.h"
#include "Constants.h"

void SendJoystick(void){
  esp_now_send(Reciever.peer_addr, (uint8_t *)&Controller, sizeof(Controller));
}

void SendCallback(const uint8_t *mac_addr, esp_now_send_status_t status){
  if(status == ESP_NOW_SEND_SUCCESS){
    connection_Established = true;
  } 
  else connection_Established = false;
}

void AttemptConnection(void){
  uint8_t RecieverMAC[6] = {0xB0, 0xA7, 0x32, 0xC3, 0x6B, 0xDC}; 
  memcpy(Reciever.peer_addr, RecieverMAC, 6);
  Reciever.channel = ESPNOW_CHANNEL;
  Reciever.encrypt = 0;
  esp_now_del_peer(Reciever.peer_addr);
  esp_now_add_peer(&Reciever);
}

/*void ScanForSlave(void){
  int8_t scanResults = WiFi.scanNetworks();

  for (int i=0; i<scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    String BSSIDstr = WiFi.BSSIDstr(i);
    if (SSID.indexOf("RX") == 0) {
      int mac[6];
      if (6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])) {
      for (int ii = 0; ii < 6; ++ii) { Reciever.peer_addr[ii] = (uint8_t)mac[ii]; }
      }
      Reciever.channel = ESPNOW_CHANNEL; // pick a channel
      Reciever.encrypt = 0;              // no encryption
      break;
    }
  }
}*/

/*void AttemptConnection(void){
  WriteText("Attempting","Connection","To Peer");
  ScanForSlave();
  esp_now_del_peer(Reciever.peer_addr);
  esp_now_add_peer(&Reciever);
}*/
