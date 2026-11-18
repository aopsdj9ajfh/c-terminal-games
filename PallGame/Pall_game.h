#pragma once

#define WIDTH 60
#define HEIGHT 25

typedef struct {
    int y;
    int height;
} Paddle;

typedef struct {
    int x,y;
    int dx,dy;
    int isActive;
} Ball;

extern Paddle paddle;
extern Ball ball[50];
extern int ballCount;
extern int score;
extern int gameSpeed;

void InitGame();
void DrawGame();
void UpdateGame();
void GameRun();