#pragma once

#define WIDTH 40
#define HEIGHT 20

typedef struct {
    int y;
    int velocity;
} Bird;

typedef struct {
    int x;
    int gapY;
    int gapSize;
} Pipe;

extern Bird bird;
extern Pipe pipe;
extern int score;
extern int isGameOver;

void InitGame();
void DrawGame();
void UpdateGame();
void GameRun();