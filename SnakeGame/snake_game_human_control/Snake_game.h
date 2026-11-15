#pragma once

#define WIDTH 30
#define HEIGHT 20

#define INIT_LEN 3

void InitGame();
void DrawMap();
void DrawSnake();
void MoveSnake();
void EatFood();
void DrawFood();
void DrawScore();
void DrawGameOver();
void KeyControl();
void GameRun();