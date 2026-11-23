#pragma once

#define WIDTH 40
#define HEIGHT 25
#define MAX_ENEMY 8
#define MAX_PLAYER_BULLET 12
#define MAX_ENEMY_BULLET 8

typedef struct {
    int x;
    int y;
    int hp;
    int isActive;
} Plane;

typedef struct {
    int x;
    int y;
    int isActive;
} Bullet;

extern Plane player;
extern Plane enemyArr[MAX_ENEMY];
extern Bullet playerBulletArr[MAX_PLAYER_BULLET];
extern Bullet enemyBulletArr[MAX_ENEMY_BULLET];
extern int score;

void HideCursor();
void SetPos(int x,int y);

void InitGame();
void Draw();
void Input();
void Logic();

void SpawnEnemy();
void PlayerFire();
void EnemyFire();
void EnemyLogic();
void BulletLogic();
void ResetBullet(Bullet* b);
void GameRun();