#pragma once

#define ROW 10
#define COM 12

extern char map[ROW][COM];
extern int px, py;

void ShowMap();
void FindPlayer();
int  CheckWin();
void Move(int dx,int dy);
void Reset();
void GameRun();