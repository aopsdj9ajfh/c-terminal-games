#pragma once

#define WIDTH 30
#define HEIGHT 20

#define INIT_LEN 3

extern int isAutoMode; //控制模式：0.人工 1.自动

void InitGame();
void DrawMap();
void DrawSnake();
void MoveSnake();
void AutoPilot();
void EatFood();
void DrawFood();
void DrawScore();
void DrawGameOver();
void KeyControl();
void GameRun();