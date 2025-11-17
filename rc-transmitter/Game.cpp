#include <stdio.h>
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
#include "Game.h"

uint8_t current_Game = GAME_NONE;
uint8_t current_MenuIndex = 0;

unsigned long previous_TimeGame = 0;

void PlayGame(void) {
  unsigned long current_Time = millis();

  char Line1[32], Line2[32], Line3[32], Line4[32];

  sprintf(Line1, current_MenuIndex == GAME_TICTACTOE ? "TicTacToe [x]" : "TicTacToe [ ]");
  sprintf(Line2, current_MenuIndex == GAME_CATCH      ? "Catch [x]"      : "Catch [ ]");
  sprintf(Line3, current_MenuIndex == GAME_DINO      ? "Dino [x]"      : "Dino [ ]");
  sprintf(Line4, current_MenuIndex == GAME_EXIT      ? "Exit [x]"      : "Exit [ ]");

  WriteText2(Line1, Line2, Line3, Line4);

  if(MapJoystickToDirection(Controller.value_MJX, Controller.value_MJY) == DIRECTION_DOWN && current_Time - previous_TimeGame >= debounce_Time) {
    current_MenuIndex = (current_MenuIndex + 1) % 4;
    previous_TimeGame = current_Time;
  } 
  else if(MapJoystickToDirection(Controller.value_MJX, Controller.value_MJY) == DIRECTION_UP && current_Time - previous_TimeGame >= debounce_Time) {
    current_MenuIndex = (current_MenuIndex + 3) % 4; 
    previous_TimeGame = current_Time;
  }

  if(MapJoystickToDirection(Controller.value_MJX, Controller.value_MJY) == DIRECTION_RIGHT && current_Time - previous_TimeGame >= debounce_Time){
    previous_TimeGame = current_Time;
    switch (current_MenuIndex){
      case GAME_TICTACTOE: 
        TicTacToe(); 
        break;
      case GAME_CATCH:      
        CatchBall();      
        break;
      case GAME_DINO:      
        Dino();      
        break;
      case GAME_EXIT:      
        current_Game = GAME_NONE; 
        current_State = STATE_DISPLAY; 
        break;
    }
  }
}

void TicTacToe(void){
  uint8_t grid_TicTacToe[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
  bool player_XTurn = true;
  uint8_t current_GridRow = 0;
  uint8_t current_GridCol = 0;
  uint8_t current_Winner = 0;
  unsigned long previous_PressTime = 0;
  
  while(!ButtonHeldFor(Controller.value_MSW, hold_Time)){
    unsigned long current_Time = millis();
    
    UpdateJoystick();

    LedDisplay.clearDisplay();

    LedDisplay.drawFastVLine(15, 1, 30, SSD1306_WHITE);
    LedDisplay.drawFastVLine(25, 1, 30, SSD1306_WHITE);
    LedDisplay.drawFastHLine(5, 11, 30, SSD1306_WHITE);
    LedDisplay.drawFastHLine(5, 21, 30, SSD1306_WHITE);

    LedDisplay.setTextSize(1);
    LedDisplay.setTextColor(SSD1306_WHITE);
    LedDisplay.setCursor(40, 3);
    
    if(player_XTurn){ LedDisplay.println("X's Turn"); }
    else LedDisplay.println("O's Turn");

    LedDisplay.setCursor(40, 23);

    switch(current_Winner){
      case 'X': LedDisplay.println("Winner : X"); break;
      case 'O': LedDisplay.println("Winner : O"); break;
      default:  LedDisplay.println("Winner : None"); break;
    }

    if(current_Time - previous_TimeGame >= debounce_Time){
      switch( MapJoystickToDirection(Controller.value_MJX, Controller.value_MJY) ){
        case DIRECTION_DOWN:   current_GridRow = (current_GridRow + 1) % 3; break;
        case DIRECTION_UP:     current_GridRow = (current_GridRow + 2) % 3; break;
        case DIRECTION_RIGHT:  current_GridCol = (current_GridCol + 1) % 3; break;
        case DIRECTION_LEFT:   current_GridCol = (current_GridCol + 2) % 3; break;
        default: break;
      }
      previous_TimeGame = current_Time;
    }
    
    uint8_t x = 8 + current_GridCol*10;
    uint8_t y = 3 + current_GridRow*10;
    LedDisplay.drawChar(x, y, '#', SSD1306_WHITE, 1, 1);

    if(Controller.value_MSW && grid_TicTacToe[current_GridRow][current_GridCol] == 0 && current_Time - previous_PressTime >= debounce_Time && current_Winner == 0){
      if(player_XTurn){
        grid_TicTacToe[current_GridRow][current_GridCol] = 'X';
        player_XTurn = false;
      }
      else{
        grid_TicTacToe[current_GridRow][current_GridCol] = 'O';
        player_XTurn = true; 
      }

      previous_PressTime = current_Time;
    }

    for(int row=0; row<3; row++){
      for(int col=0; col<3; col++){
        if(grid_TicTacToe[row][col] == 'X'){
          uint8_t x = 8 + col*10;
          uint8_t y = 3 + row*10;
          LedDisplay.drawChar(x, y, 'X', SSD1306_WHITE, 1, 1);
        }
        else if(grid_TicTacToe[row][col] == 'O'){
          uint8_t x = 8 + col*10;
          uint8_t y = 3 + row*10;
          LedDisplay.drawChar(x, y, 'O', SSD1306_WHITE, 1, 1); 
        }
      }
    }

    current_Winner = CheckWinner(grid_TicTacToe);

    if(Controller.value_MSW && current_Time - previous_PressTime >= debounce_Time && current_Winner != 0){
      for(int row=0; row<3; row++){
        for(int col=0; col<3; col++){
        grid_TicTacToe[row][col] = 0;
        }
      }

      player_XTurn = true;
      current_GridRow = 0;
      current_GridCol = 0;
      current_Winner = 0; 
      previous_PressTime = current_Time;
    }

    LedDisplay.display();
  }
}

char CheckWinner(uint8_t grid[3][3]) {
  for (int row = 0; row < 3; row++) {
    if (grid[row][0] != 0 &&
      grid[row][0] == grid[row][1] &&
      grid[row][1] == grid[row][2])
      return grid[row][0]; 
    }
  for (int col = 0; col < 3; col++) {
    if (grid[0][col] != 0 &&
      grid[0][col] == grid[1][col] &&
      grid[1][col] == grid[2][col])
      return grid[0][col];
    }
    if (grid[0][0] != 0 &&
      grid[0][0] == grid[1][1] &&
      grid[1][1] == grid[2][2])
      return grid[0][0];
    if (grid[0][2] != 0 &&
        grid[0][2] == grid[1][1] &&
        grid[1][1] == grid[2][0])
        return grid[0][2];
  return 0;
}

void CatchBall(void){
  unsigned long previous_TimeGame = 0;
  const uint8_t h_Paddle = 7;
  uint8_t x_Paddle = 115;
  uint8_t y_Paddle = 16;
  uint8_t score = 0;
  char score_Text[4] = {0,0,0,0};
  uint8_t x_Ball = 0;
  uint8_t y_Ball = random(0, 32);
  uint8_t s_Ball = random(1,5);
  while(!ButtonHeldFor(Controller.value_MSW, hold_Time)){
    unsigned long current_Time = millis();

    UpdateJoystick();
    
    LedDisplay.clearDisplay();
    
    LedDisplay.setTextSize(1);
    LedDisplay.setTextColor(SSD1306_WHITE);
    
    sprintf(score_Text, "%d", score);

    LedDisplay.drawChar(120, 0, score_Text[0], SSD1306_WHITE, 1, 1); 
    LedDisplay.drawChar(120, 9, score_Text[1], SSD1306_WHITE, 1, 1); 
    LedDisplay.drawChar(120, 17, score_Text[2], SSD1306_WHITE, 1, 1); 
    LedDisplay.drawChar(120, 25, score_Text[3], SSD1306_WHITE, 1, 1); 

    LedDisplay.drawFastVLine(x_Paddle, y_Paddle, h_Paddle, SSD1306_WHITE);
    LedDisplay.drawFastVLine(x_Paddle+1, y_Paddle, h_Paddle, SSD1306_WHITE);

    if(x_Ball == 115){
      if(y_Ball >= y_Paddle && y_Ball <= y_Paddle+h_Paddle-1){
        score++;
      }
      s_Ball = random(1, 4);
      y_Ball = random(0, 32);
      x_Ball = 0;
    }
    x_Ball = x_Ball + s_Ball;
    x_Ball = constrain(x_Ball, 0, 115);

    if(current_Time - previous_TimeGame >= 10){
      switch( MapJoystickToDirection(Controller.value_MJX, Controller.value_MJY) ){
        case DIRECTION_DOWN:   y_Paddle = constrain(y_Paddle+1, 0, 25); break;
        case DIRECTION_UP:     y_Paddle = constrain(y_Paddle-1, 0, 25); break;
        default: break;
      }
      previous_TimeGame = current_Time;
    }

    LedDisplay.drawPixel(x_Ball, y_Ball, SSD1306_WHITE);
    LedDisplay.display();
  }
}

void Dino(void) {
  while(!ButtonHeldFor(Controller.value_MSW, hold_Time)){
    UpdateJoystick();
    WriteText("Dino", "Tac", "Toe");
  }
}

bool ButtonHeldFor(uint8_t value_Button, unsigned long hold_Time) {
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