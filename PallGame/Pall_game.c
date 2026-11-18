#include "Pall_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

Paddle paddle;
Ball ball[50];
int ballCount = 0;
int score = 0;
int gameSpeed = 0;
int frameCount = 0;
int lastRewardScore = 0;

//光标控制
void SetPos(int x, int y){
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void HideCursor(){
    CONSOLE_CURSOR_INFO ci = {1 , 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

void InitGame(){
    HideCursor();

    paddle.y = HEIGHT / 2;
    paddle.height = 6;

    ballCount = 1;
    score = 0;
    gameSpeed = 0;
    frameCount = 0;

    ball[0].x = WIDTH - 5;
    ball[0].y = HEIGHT/2;
    ball[0].dx = -2;
    ball[0].dy = 2;
    ball[0].isActive = 1;
}

void DrawGame(){
    SetPos(0, 0);
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (j == 0 || j == WIDTH - 1 || i == 0 || i == HEIGHT - 1){
                printf("#");
            }else if (j == 2 && i >= paddle.y && i < paddle.y + paddle.height){
                printf("|");
            }else {
                printf(" ");
            }
        }
        printf("\n");
    }

    //画球(因为球可以重叠，所以单独遍历画)
    for (int i = 0; i < ballCount; i++){
        SetPos(ball[i].x, ball[i].y);
        printf("O");
    }
    
    //打印状态栏
    SetPos(0, HEIGHT);
    printf("Score: %d | Balls: %d | speed: %d", score, ballCount, gameSpeed);
}

void UpdateGame(){
    frameCount++;

    //1.鼠标控制挡板
    POINT p;
    GetCursorPos(&p);
    //获取控制台窗口坐标
    HWND hwnd = GetConsoleWindow();
    ScreenToClient(hwnd, &p);

    //将屏幕像素坐标转化为控制台字符坐标（大约除以 16 ~ 20 的字体高度）
    CONSOLE_FONT_INFO cfi;
    GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    int charHeight = cfi.dwFontSize.Y;
    int mouseCharY = p.y / charHeight;
    if (mouseCharY > 0 && mouseCharY < HEIGHT - paddle.height){
        paddle.y = mouseCharY;
    }
    
    for (int i = 0; i < ballCount; i++){
        if (!ball[i].isActive) continue;
        ball[i].y += ball[i].dy;
        ball[i].x += ball[i].dx;
        
        if (ball[i].y < 1 || ball[i].y >= HEIGHT - 2){
            ball[i].dy *= -1;
        }
        if (ball[i].x >= WIDTH - 2){
            ball[i].dx *= -1;
        }
        if (ball[i].x <= 2){
            if (ball[i].y > paddle.y && ball[i].y < paddle.y + paddle.height){
                ball[i].dx *= -1;
                score++;
            }else{
                ball[i].isActive = 0;
                SetPos(WIDTH/2 + 5, HEIGHT/2);
                printf("Game Over! Final Score: %d", score);
                exit(0);
            }
        }
    }
    
    if (score != 0 && score%3 == 0 && score != lastRewardScore){
        lastRewardScore = score;
        gameSpeed++;
        if (ballCount < 10){
            ballCount++;
            ball[ballCount - 1].x = WIDTH - 5;
            ball[ballCount - 1].y = rand() % (HEIGHT - 4) + 2;
            ball[ballCount - 1].dx = -1;
            ball[ballCount - 1].dy = ballCount%2 == 0? 1: -1;
            ball[ballCount - 1].isActive = 1; 
        }
    }
}

void GameRun(){
    InitGame();
    while(1){
        DrawGame();
        UpdateGame();
        Sleep(100 - gameSpeed * 5);
    }
}