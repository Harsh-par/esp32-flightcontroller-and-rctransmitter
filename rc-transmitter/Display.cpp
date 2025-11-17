#include <stdint.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>

#include "Display.h"
#include "Globals.h"
#include "Constants.h"

void WriteText(const char* Line1, const char* Line2, const char* Line3) {
  LedDisplay.clearDisplay();
  LedDisplay.setTextSize(1);
  LedDisplay.setTextColor(SSD1306_WHITE);
  LedDisplay.setCursor(0, 2);
  LedDisplay.println(Line1);
  LedDisplay.setCursor(0, 13);
  LedDisplay.println(Line2);
  LedDisplay.setCursor(0, 24);
  LedDisplay.println(Line3);
  //LedDisplay.invertDisplay(true);
  LedDisplay.display();
}

void WriteText2(const char* Line1, const char* Line2, const char* Line3, const char* Line4){
  LedDisplay.clearDisplay();
  LedDisplay.setTextSize(1);
  LedDisplay.setTextColor(SSD1306_WHITE);
  LedDisplay.setCursor(0, 0);
  LedDisplay.println(Line1);
  LedDisplay.setCursor(0, 9);
  LedDisplay.println(Line2);
  LedDisplay.setCursor(0, 17);
  LedDisplay.println(Line3);
  LedDisplay.setCursor(0, 25);
  LedDisplay.println(Line4);
  //LedDisplay.invertDisplay(true);
  LedDisplay.display();
}

int MapJoystick(int Raw, int Center, int RangeNeg, int RangePos, int Deadzone){
  int Value = Raw - Center;

  if(abs(Value) < Deadzone) return 0;

  if(Value < 0)
    Value = map(Value, -RangeNeg, 0, -100, 0);
  else
    Value = map(Value, 0, RangePos, 0, 100);

  return constrain(Value, -100, 100);
}

void ProcessJoystick(void){
  char Line1[32];
  sprintf(Line1, "%4d : %4d : %1d", Controller.value_LJX, Controller.value_LJY, Controller.value_LSW);
  char Line2[32];
  sprintf(Line2, "%4d : %4d : %1d", Controller.value_MJX, Controller.value_MJY, Controller.value_MSW);
  char Line3[32];
  if(connection_Established){
  sprintf(Line3, "%4d : %4d : %1d  [x]", Controller.value_RJX, Controller.value_RJY, Controller.value_RSW);
  }
  else sprintf(Line3, "%4d : %4d : %1d  [ ]", Controller.value_RJX, Controller.value_RJY, Controller.value_RSW);

  WriteText(Line1, Line2, Line3);
}

void UpdateJoystick(void){
  unsigned long current_Time = millis();

  Controller.value_LJX = MapJoystick(analogRead(pin_LJX));
  Controller.value_LJY = MapJoystick(analogRead(pin_LJY));
  Controller.value_MJX = MapJoystick(analogRead(pin_MJX));
  Controller.value_MJY = MapJoystick(analogRead(pin_MJY));
  Controller.value_RJX = MapJoystick(analogRead(pin_RJX));
  Controller.value_RJY = MapJoystick(analogRead(pin_RJY));

  Controller.value_LSW = !digitalRead(pin_LSW);
  Controller.value_MSW = !digitalRead(pin_MSW);
  Controller.value_RSW = !digitalRead(pin_RSW);

  if(Controller.value_LSW && (current_Time - previous_PressA > debounce_Time)){
    previous_PressA = current_Time;
  }
  if(Controller.value_MSW && (current_Time - previous_PressB > debounce_Time)){
    previous_PressB = current_Time;
  }
  if(Controller.value_RSW && (current_Time - previous_PressC > debounce_Time)){
    previous_PressC = current_Time;
  }
}

int MapJoystickToDirection(int joystick_X, int joystick_Y) {
  if (abs(joystick_X) < JOYSTICK_DEADZONE && abs(joystick_Y) < JOYSTICK_DEADZONE) return DIRECTION_CENTER;
  if (joystick_Y > 0 && abs(joystick_Y) > abs(joystick_X)) return DIRECTION_UP;
  if (joystick_Y < 0 && abs(joystick_Y) > abs(joystick_X)) return DIRECTION_DOWN;
  if (joystick_X > 0 && abs(joystick_X) > abs(joystick_Y)) return DIRECTION_RIGHT;
  if (joystick_X < 0 && abs(joystick_X) > abs(joystick_Y)) return DIRECTION_LEFT;
  return DIRECTION_CENTER;
}