#include "plane.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

Plane player;
Plane enemyArr[MAX_ENEMY];
Bullet playerBulletArr[MAX_PLAYER_BULLET];
Bullet enemyBulletArr[MAX_ENEMY_BULLET];

int frameCount = 0;
int score;
char screen[HEIGHT][WIDTH + 1];

//工具函数
void SetPos(int x, int y){
    COORD pos = {(SHORT) x,(SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void HideCursor(){
    CONSOLE_CURSOR_INFO cursor = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

//功能函数
void SpawnEnemy(){
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (enemyArr[i].isActive == 0) {
            enemyArr[i].x = rand() % WIDTH;
            enemyArr[i].y = 0;
            enemyArr[i].hp = 2;
            enemyArr[i].isActive = 1;
            break;
        }
    }
}

void ResetBullet(Bullet* b){
    b->isActive = 0;
    b->x = -1;
    b->y = -1;
}

void BulletLogic(){
    for (int i = 0; i < MAX_PLAYER_BULLET; i++) {
        Bullet* b = &playerBulletArr[i];
        if (b->isActive) {
            b->y--;
            if (b->y < 0) {
                ResetBullet(b);
                continue;
            }
            for (int e = 0; e < MAX_ENEMY; e++) {
                Plane *en = &enemyArr[e];
                if (b->x == en->x && b->y == en->y && en->isActive) {
                    en->hp--;
                    ResetBullet(b);
                    if (en->hp <= 0) {
                        en->isActive = 0;
                        score += 50;
                    }
                    break;
                }
            }
        }
    }
    for (int i = 0; i < MAX_ENEMY_BULLET; i++) {
        Bullet* b = &enemyBulletArr[i];
        if (b->isActive) {
            b->y++;
            if (b->y >= HEIGHT) {
                ResetBullet(b);
                continue;
            }
            if (b->x == player.x && b->y == player.y && player.isActive) {
                player.hp--;
                ResetBullet(b);
                if (player.hp <= 0) {
                    player.isActive = 0;
                    SetPos(WIDTH/2 - 6, HEIGHT/2);
                    printf("游戏结束！最终得分: %d", score);
                    getchar();
                    exit(0);
                }
            }
        }
    }
}

void EnemyLogic(){
    for (int i = 0; i < MAX_ENEMY; i++) {
        frameCount++;
        Plane* en = &enemyArr[i];
        if (en->isActive) {
            if (frameCount % 3 == 0) en->y++;
            if (en->y > HEIGHT) {
                en->isActive = 0;
                score += 1;
            }
            if (en->x == player.x && en->y == player.y && player.isActive) {
                player.hp -= 2;
                en->isActive = 0;
                if (player.hp <= 0) {
                    SetPos(WIDTH/2 - 6, HEIGHT/2);
                    printf("游戏结束! 最终得分: %d", score);
                    getchar();
                    exit(0);
                }
            }
        }
    }
}

void PlayerFire(){
    for (int i = 0; i < MAX_PLAYER_BULLET; i++){
        if (playerBulletArr[i].isActive == 0) {
            playerBulletArr[i].x = player.x;
            playerBulletArr[i].y = player.y - 1;
            playerBulletArr[i].isActive = 1;
            break;
        }
    }
}

void EnemyFire(){
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (enemyArr[i].isActive) {
            if (rand()%100 <= 8) {
                for (int j = 0; j < MAX_ENEMY_BULLET; j++) {
                    if (enemyBulletArr[j].isActive == 0) {
                        enemyBulletArr[j].x = enemyArr[i].x;
                        enemyBulletArr[j].y = enemyArr[i].y + 1;
                        enemyBulletArr[j].isActive = 1;
                        break;
                    }
                }
            }
        }
    }
}

void InitGame(){
    player.x = WIDTH/2;
    player.y = HEIGHT - 2;
    player.hp = 5;
    player.isActive = 1;

    for (int i = 0; i < MAX_ENEMY; i++) enemyArr[i].isActive = 0;
    for (int i = 0; i < MAX_ENEMY_BULLET; i++) ResetBullet(&enemyBulletArr[i]);
    for (int i = 0; i < MAX_PLAYER_BULLET; i++) ResetBullet(&playerBulletArr[i]);

    score = 0;
    HideCursor();
}

void Draw(){
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            screen[i][j] = ' ';
        }
        screen[i][WIDTH] = '\0';
    }
    if (player.isActive) screen[player.y][player.x] = 'A';
    for (int i = 0; i < MAX_ENEMY; i++) if (enemyArr[i].isActive) screen[enemyArr[i].y][enemyArr[i].x] = 'V';
    for (int i = 0; i < MAX_PLAYER_BULLET; i++) {Bullet* b = &playerBulletArr[i]; if (b->isActive) screen[b->y][b->x] = '*';}
    for (int i = 0; i < MAX_ENEMY_BULLET; i++) {Bullet* b = &enemyBulletArr[i]; if (b->isActive) screen[b->y][b->x] = 'o';}
    
    SetPos(0,0);
    for (int i = 0; i < HEIGHT; i++){
        printf("%s\n", screen[i]);
    }
    printf("分数: %d | 玩家HP: %d | W上S下A左D右 空格发射 Esc退出\n", score, player.hp);
}

void Input(){
    if (_kbhit()){
        int ch = _getch();
        switch(ch){
            case 'W':case 'w': if (player.y>0)        player.y--; break;
            case 'S':case 's': if (player.y<HEIGHT-1) player.y++; break;
            case 'A':case 'a': if (player.x>0)        player.x--; break;
            case 'D':case 'd': if (player.x<WIDTH-1)  player.x++; break;
            case ' ':PlayerFire();break;
            case 27: exit(0);
        }
    }
}

void Logic(){
    if (rand()%100 <= 12) SpawnEnemy();
    EnemyLogic();
    EnemyFire();
    BulletLogic();
}

void GameRun(){
    InitGame();
    while(1){
        Draw();
        Input();
        Logic();
        Sleep(70);
    }
}