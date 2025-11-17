#pragma once
#include <Arduino.h>

void PlayGame(void);
void TicTacToe(void);
char CheckWinner(uint8_t grid[3][3]);
void CatchBall(void);
void Dino(void);
bool ButtonHeldFor(uint8_t value_Button, unsigned long hold_Time);