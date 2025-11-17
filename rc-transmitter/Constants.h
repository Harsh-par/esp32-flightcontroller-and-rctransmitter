#pragma once

//#define DEBUG_MODE  1
#define ESPNOW_MODE 1

#define ESPNOW_NETWORK  "RX_1"
#define ESPNOW_PASSWORD "RX_1_Password"
#define ESPNOW_CHANNEL  1

#define SCREEN_ADDRESS 0x3C 
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  32

#define ESP32_CORE1 0
#define ESP32_CORE2 1

#define STATE_DISPLAY 0
#define STATE_GAME    1

#define GAME_TICTACTOE 0
#define GAME_CATCH     1
#define GAME_DINO      2
#define GAME_EXIT      3
#define GAME_NONE      4

#define JOYSTICK_DEADZONE 5

#define DIRECTION_CENTER  0
#define DIRECTION_UP      1
#define DIRECTION_DOWN    2
#define DIRECTION_LEFT    3
#define DIRECTION_RIGHT   4

#define BAUD_RATE 115200

constexpr unsigned long debounce_Time = 175;
constexpr unsigned long delay_Time    = 1200;
constexpr unsigned long hold_Time     = 1000;

//ESPNOW MAC Address
constexpr uint8_t reciever_MAC[6] = {0xB0, 0xA7, 0x32, 0xC3, 0x6B, 0xDC}; 

//Pin for onboard LED
constexpr int pin_Led = 2;

//Pins for I2C
constexpr int pin_Sda = 21, pin_Scl = 22;

//Pins for Joysticks
constexpr int pin_RJX = 33, pin_RJY = 32, pin_RSW = 4;
constexpr int pin_MJX = 35, pin_MJY = 34, pin_MSW = 16;
constexpr int pin_LJX = 39, pin_LJY = 36, pin_LSW = 15;
