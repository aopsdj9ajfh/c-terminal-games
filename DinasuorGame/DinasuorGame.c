#include "DinasuorGame.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

Dino dino;
Cactus cactus;
int gameSpeed;
int score;
int lastSpeedUpScore;

//工具函数
void SetPos(int x, int y){
    COORD pos = {(SHORT)x,(SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void InitGame(){
    dino.y = GROUND_Y;
    dino.velocity = 0;
    dino.isJumping = 0;

    cactus.x = WIDTH - 5;
    cactus.y = GROUND_Y;
    cactus.width = 2;
    cactus.height = 2;
    cactus.isActive = 1;

    gameSpeed = 2;
    score = 0;
}

void DrawGame(){
    SetPos(0,0);
    for (int i = 0;i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (i == dino.y && j == 5){
                printf("D");
            }else if (i == GROUND_Y){
                printf("-");
            }else if (i >= cactus.y - cactus.height && i <= cactus.y && j >= cactus.x && j < cactus.x + cactus.width){
                printf("|");
            }else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("Score: %d | Speed: %d", score, gameSpeed);
}

void UpdateGame(){
    if (dino.isJumping){
        dino.y += dino.velocity;
        dino.velocity++;
        if (dino.y >= GROUND_Y){
            dino.y = GROUND_Y;
            dino.velocity = 0;
            dino.isJumping = 0;
        }
    }

    if (cactus.isActive){
        cactus.x -= gameSpeed;
        if (cactus.x < 0){
            score += 10;
            cactus.x = WIDTH - 5;
            cactus.height = rand() % 3 + 2;
        }
    }

    if (score > 0 && score % 50 == 0 && score != lastSpeedUpScore){
        gameSpeed++;
        lastSpeedUpScore = score;
    }
}

void HandleGame(){}

int CheckCollision(){
    if (cactus.isActive){
        if (cactus.x <= 5 && cactus.x + cactus.width > 5){
            if (dino.y >= cactus.y - cactus.height + 1){
                return 1; //撞上了
            }
        }
    }
    return 0;
}

void GameRun(){
    while (1){
        if (_kbhit()){
            int ch = _getch();
            if ((ch == 'w' || ch == 'W' || ch == ' ') && !dino.isJumping){
                dino.isJumping = 1;
                dino.velocity = -3;
            }
        }

        UpdateGame();

        if (CheckCollision()){
            printf("\n\nGame Over! Final Score: %d", score);
            return;
        }
        DrawGame();
        Sleep(100 - gameSpeed * 5);
    }
}