#pragma once

#define WIDTH 50
#define HEIGHT 15
#define GROUND_Y 12

//恐龙属性
typedef struct {
    int y;
    int velocity;
    int isJumping;
} Dino;

//仙人掌属性
typedef struct {
    int x;
    int y;
    int width;
    int height;
    int isActive;
} Cactus;

extern Dino dino;
extern Cactus cactus;
extern int gameSpeed;
extern int score;

void InitGame();
void DrawGame();
void UpdateGame();
void HandleGame();
int CheckCollision();
void GameRun();