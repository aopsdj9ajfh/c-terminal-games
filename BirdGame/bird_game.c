#include "bird_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

Bird bird;
Pipe pipe;
int score = 0;
int isGameOver = 0;

//光标控制
void SetPos(int x, int y){
    COORD pos = {(SHORT) x,(SHORT) y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void HideCursor(){
    CONSOLE_CURSOR_INFO ci = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

void InitGame(){
    HideCursor();
    bird.y = HEIGHT/2;
    bird.velocity = 0;

    pipe.x = WIDTH - 10;
    pipe.gapY = HEIGHT/2;
    pipe.gapSize = 5;
    
    score = 0;
    isGameOver = 0;
}

void DrawGame(){
    SetPos(0,0);
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1){
                printf("#");
            }else if (j == 5 && i == bird.y){
                if (bird.velocity < 0) printf("^");
                else printf(">");
            }else if (j == pipe.x){
                int gapTop = pipe.gapY - pipe.gapSize/2;
                int gapButton = pipe.gapY + pipe.gapSize/2;
                if (i < gapTop || i > gapButton){
                    printf("|");
                }else {
                    printf(" ");
                }
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }
    SetPos(0, HEIGHT);
    printf("Score: %d",score);
}

void UpdateGame(){
    if (isGameOver) return;

    bird.velocity += 1;
    bird.y += bird.velocity;

    if (bird.y <= 1 || bird.y >= HEIGHT - 2){
        isGameOver = 1;
    }

    pipe.x -= 1;
    if (pipe.x <= 0){
        score += 10;
        pipe.x = WIDTH - 5;
        pipe.gapY = rand() % (HEIGHT - pipe.gapSize - 2) + pipe.gapSize/2 + 1;
    }
    
    if (pipe.x == 5 || pipe.x == 6){
    int gapTop = pipe.gapY - pipe.gapSize/2;
    int gapButton = pipe.gapY + pipe.gapSize/2; 
    if (bird.y < gapTop || bird.y > gapButton){
        isGameOver = 1;
        }
    }
}

void GameRun(){
    InitGame();
    while(1){
        if (_kbhit()){
            int ch = _getch();
            if ((ch == ' ' || ch == 'w' || ch == 'W' ) && !isGameOver){
                bird.velocity = -2;
            }
        }
        UpdateGame();
        DrawGame();
        
        if (isGameOver){
            SetPos(WIDTH/2 - 5, HEIGHT/2);
            printf("Game Over!");
            SetPos(WIDTH/2 - 8, HEIGHT/2 + 1); 
            printf("Final Score: %d",score);
            break;
        }
        Sleep(80);
    }
}