#pragma once
#include <Arduino.h>

void WriteText(const char* Line1, const char* Line2, const char* Line3);
void WriteText2(const char* Line1, const char* Line2, const char* Line3, const char* Line4);
int MapJoystick(int Raw, int Center = 1800, int RangeNeg = 1800, int RangePos = 2000, int Deadzone = 100);
int MapJoystickToDirection(int joystick_X, int joystick_Y);
void ProcessJoystick(void);
void UpdateJoystick(void);
void PlayGame(void);
