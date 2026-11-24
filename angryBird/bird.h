#pragma once

#define SCREEN_W 80
#define SCREEN_H 24
#define G 0.18f
#define SPEED_K 0.35f
#define BIRD_CHAR '@'
#define BLOCK_CHAR '#'
#define EMPTY ' '

typedef struct {
    float x,y;
    float vx,vy;
    int fly;
} Bird;

typedef struct {
    int x,y;
    int alive;
} Block;

typedef struct {
    int down;
    int sx,sy;
    int ex,ey;
} Mouse;

extern Bird bird;
extern Block block[50];
extern int blockCnt;
extern int score;

void SetPos(int x, int y);
void HideCursor();
void GetMouse(Mouse* m);
void DisableQuickEditMode();

void InitGame();
void InitBird();
void InitBlock();
void Draw();
void CheckCollision();
void Update();
void CheckGameWin();
void GameRun();