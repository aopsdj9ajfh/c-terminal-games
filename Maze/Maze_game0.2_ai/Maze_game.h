#pragma once

#define WIDTH 21
#define HEIGHT 21

extern int maze[HEIGHT][WIDTH];
extern int playerX, playerY;
extern int isAutoMode;
extern int dx[4];
extern int dy[4];
extern int stackX[WIDTH * HEIGHT];
extern int stackY[WIDTH * HEIGHT];
extern int top;

void InitMaze();
void GenerateMaze();
void DrawMaze();
void GameRun();
void HandleManualMode(int ch);
void HandleAutoMode();
void ChangeMode(int ch);
void HideCursor();