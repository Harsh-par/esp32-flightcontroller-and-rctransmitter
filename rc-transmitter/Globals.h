#pragma once

#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include "Controller_Data.h"

extern ControllerData Controller;
extern Adafruit_SSD1306 LedDisplay;
extern esp_now_peer_info_t Reciever;

extern bool connection_Established;

extern unsigned long previous_PressA;
extern unsigned long previous_PressB;
extern unsigned long previous_PressC;
extern unsigned long previous_TimeTaskA;
extern unsigned long previous_TimeTaskB;

extern int current_State;