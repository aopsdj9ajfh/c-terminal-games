#include "Snake_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

//全局变量定义
int snakeX[100], snakeY[100];
int snakeLength;
int direction;
int foodX, foodY;
int score;
int gameOver;

//底层工具函数
//隐藏光标
void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
//设置光标位置
void SetPos(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//内部辅助函数
//生成不重叠的食物
void GenerateFood(){
    bool valid;
    do {
        valid = true;
        foodX = rand() % (WIDTH - 2) + 1;
        foodY = rand() % (HEIGHT-2) + 1;
        for (int i = 0;i < snakeLength;i++){
            if (foodX == snakeX[i] && foodY == snakeY[i]){
                valid = false;
                break;
            }
        }
    }while(!valid);
}

//核心功能函数实现
//初始化游戏
void InitGame() {
    HideCursor();
    gameOver = 0;
    score = 0;
    direction = 4; // 初始方向向右
    snakeLength = INIT_LEN;
    for (int i = 0; i < snakeLength; i++) {
        snakeX[i] = WIDTH / 2 - i;
        snakeY[i] = HEIGHT / 2;
    }
    // 随机生成食物位置
    GenerateFood();
}

void DrawMap() {
    SetColor(7); // 墙壁默认颜色为白色
    for (int i = 0; i < WIDTH; i++){
        SetPos(i , 0); printf("#");
        SetPos(i , HEIGHT); printf("#");
    }
    for (int i = 0; i < HEIGHT; i++) {
        SetPos(0, i); printf("#");
        SetPos(WIDTH, i); printf("#");
    }
}

void DrawSnake() {
    for (int i = 0; i < snakeLength; i++) {
        SetPos(snakeX[i], snakeY[i]);
        if (i==0) {
            SetColor(10); 
            printf("@");
        }
        else {
            SetColor(2);
            printf("o");
        }
    }
    SetColor(7); // 恢复默认颜色
}

void MoveSnake() {
    // 1. 【修复残影核心】无论是否吃到食物，先无条件把当前的尾巴尖擦除（用空格覆盖）
    SetPos(snakeX[snakeLength - 1], snakeY[snakeLength - 1]);
    printf(" ");

    // 2. 计算新蛇头坐标
    int newHeadX = snakeX[0];
    int newHeadY = snakeY[0];
    switch (direction) {
        case 1: newHeadY--; break; // 上
        case 2: newHeadY++; break; // 下
        case 3: newHeadX--; break; // 左
        case 4: newHeadX++; break; // 右
    }
    
    // 3. 更新蛇身位置（从后往前移动）
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    
    // 4. 更新蛇头位置
    snakeX[0] = newHeadX;
    snakeY[0] = newHeadY;
}

void EatFood() {
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        score+=10;
        if (snakeLength < 99){
            snakeX[snakeLength]=snakeX[snakeLength-1];
            snakeY[snakeLength]=snakeY[snakeLength-1];
            snakeLength++;
        }else{
            SetPos(WIDTH / 2 + 5, HEIGHT / 2);
            SetColor(10);
            printf("YOU WIN!");
            Sleep(2000);
            SetColor(7);
            exit(0);
        }
        // 随机生成新的食物位置
        GenerateFood();
    }
}

void DrawFood() {
    SetPos(foodX, foodY);
    SetColor(12); // 红色
    printf("$");
    SetColor(7); // 恢复默认颜色
}

void DrawScore() {
    SetPos(0,HEIGHT + 1);
    SetColor(14); // 黄色
    printf("Score: %d", score);
}

void DrawGameOver() {
    SetPos(WIDTH / 2 - 5, HEIGHT / 2);
    printf("Game Over!");
    _getch();
    exit(0);
}

void KeyControl() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'W': case 'w': if (direction != 2) direction = 1; break;
            case 'S': case 's': if (direction != 1) direction = 2; break;
            case 'A': case 'a': if (direction != 4) direction = 3; break;
            case 'D': case 'd': if (direction != 3) direction = 4; break;
        }
    }
}

void GameRun() {
    InitGame();
    DrawMap();
    while (!gameOver) {
        KeyControl();
        MoveSnake();
        EatFood();
        DrawSnake();
        DrawFood();
        DrawScore();
        //检查游戏结束条件
        if (snakeX[0] <= 0 || snakeX[0] >= WIDTH || snakeY[0] <= 0 || snakeY[0] >= HEIGHT) {
            gameOver = 1;
            DrawGameOver();
        }
        for (int i = 1; i < snakeLength; i++) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
                gameOver = 1;
                DrawGameOver();
                break;
            }
        }
        Sleep(150);
    }
}