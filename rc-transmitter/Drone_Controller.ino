#include <stdint.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>

#include "Constants.h"
#include "Globals.h"
#include "Display.h"
#include "Transmit.h"
#include "Game.h"
#include "Controller_Data.h"

esp_now_peer_info_t Reciever;
Adafruit_SSD1306 LedDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ControllerData Controller;

unsigned long previous_PressA = 0;
unsigned long previous_PressB = 0;
unsigned long previous_PressC = 0;
unsigned long previous_TimeTaskA  = 0;
unsigned long previous_TimeTaskB  = 0;

bool connection_Established = false;
bool connection_Attempt = false;

int current_State = STATE_DISPLAY;

void TaskDisplay(void *pvParameters);
void TaskTransmit(void *pvParameters);

TaskHandle_t DisplayHandle = NULL;
TaskHandle_t TransmitHandle = NULL;

void setup(){
  #ifdef DEBUG_MODE
    Serial.begin(BAUD_RATE);
  #endif

  Wire.begin(pin_Sda, pin_Scl);

  pinMode(pin_Led, OUTPUT);
  pinMode(pin_LJX, INPUT);
  pinMode(pin_LJY, INPUT);
  pinMode(pin_MJX, INPUT);
  pinMode(pin_MJY, INPUT);
  pinMode(pin_RJX, INPUT);
  pinMode(pin_RJY, INPUT);
  pinMode(pin_LSW, INPUT_PULLUP);
  pinMode(pin_MSW, INPUT_PULLUP);
  pinMode(pin_RSW, INPUT_PULLUP);

  if(!LedDisplay.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 allocation failed")); while(1){
    digitalWrite(pin_Led, HIGH); delay(250);
    digitalWrite(pin_Led, LOW); delay(250);
    }
  }

  #ifdef ESPNOW_MODE
    WiFi.mode(WIFI_STA);
    esp_now_init();
    esp_now_register_send_cb(SendCallback);
    AttemptConnection();
    delay(1000);
  #endif
    
  xTaskCreatePinnedToCore(TaskDisplay, "Display", 4096, NULL, 1, &DisplayHandle, ESP32_CORE1);
  xTaskCreatePinnedToCore(TaskTransmit, "Transmit", 4096, NULL, 1, &TransmitHandle, ESP32_CORE2);
}

void loop(){
}

void TaskDisplay(void *pvParameters){
  while(1){
    unsigned long current_Time = millis(); 
    static bool button_Released = true;

    UpdateJoystick();

    if(button_Released && ButtonHeld(Controller.value_MSW, hold_Time)){
      if(current_State == STATE_DISPLAY){
        current_State = STATE_GAME;
      }
      else if(current_State == STATE_GAME){
        current_State = STATE_DISPLAY; 
      }

      button_Released = false;
    }

    if(!Controller.value_MSW){
      button_Released = true;   
    }

    switch(current_State){
      case STATE_DISPLAY:
        ProcessJoystick();
        break;
      case STATE_GAME:
        PlayGame();
        break;
      default:
        break;
    }
  
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void TaskTransmit(void *pvParameters){
  while(1){
    #ifdef ESPNOW_MODE
    unsigned long current_Time = millis();

    if(Controller.value_MSW && !connection_Established && current_Time - previous_TimeTaskB > delay_Time){
      connection_Attempt = true;
      AttemptConnection();
      previous_TimeTaskB = current_Time;
    }

    if(connection_Attempt){
      digitalWrite(pin_Led, HIGH); 
      vTaskDelay(100);
      digitalWrite(pin_Led, LOW);
      connection_Attempt = false;
    }

    if(connection_Established) SendJoystick();

    vTaskDelay(pdMS_TO_TICKS(20));
    #endif
  }
}

bool ButtonHeld(uint8_t value_Button, unsigned long hold_Time) {
  static unsigned long start_Time = 0;
  static bool press_Button = false;

  unsigned long current_Time = millis();

  if(value_Button){
    if(!press_Button){
      press_Button = true;
      start_Time = current_Time;
    }
    if(current_Time - start_Time >= hold_Time) return true;
  }  
  else{
    press_Button = false; 
    start_Time = 0;  
  }

  return false;
}