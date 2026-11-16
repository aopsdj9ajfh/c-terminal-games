#pragma once

#define WIDTH 21
#define HEIGHT 21


extern int maze[HEIGHT][WIDTH];
extern int playerX, playerY;

void InitMaze();
void GenerateMaze();
void DrawMaze();
void GameRun();