#include "Tetris_game.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>

int map[HEIGHT][WIDTH] = {0};
int score = 0;

//工具函数
void SetPos(int x,int y){
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void InitGame(){
    CONSOLE_CURSOR_INFO cursorInfo = {1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursorInfo);

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            map[i][j] = 0;
        }
    }
}

void DrawMap(){
    SetPos(0,0);
    
    for (int i = 0; i < WIDTH*2 + 2; i++) printf("#");
    printf("\n");
    
    for (int i = 0; i < HEIGHT; i++){
        printf("#");
        for (int j = 0; j < WIDTH; j++){
            if (map[i][j]==0) printf("  ");
            else printf("██");
        }
        printf("#\n");
    }

    for (int i = 0; i < WIDTH*2 + 2; i++) printf("#");
    printf("\n");
}

void GeneratePiece(Piece* p){
    static const int shapes[7][4][4] = {
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, //I型
        {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, //J型
        {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, //L型
        {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, //O型
        {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, //S型
        {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}}, //T型
        {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}  //Z型
};
    int randShape = rand() % 7;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            p->shape[i][j] = shapes[randShape][i][j];
        }
    }
    p->x = WIDTH/2 - 2;
    p->y = 0;
}

void DrawPiece(Piece* p){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (p->shape[i][j] != 0){
                SetPos((p->x+j)*2 + 1,p->y+i+1);
                printf("██");
            }
        }
    }
}

void DrawScore(){
    SetPos(26,2);
    printf("score: %d", score);
}

void ClearPiece(Piece* p){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (p->shape[i][j] != 0){
                SetPos((p->x + j)*2 + 1, p->y + i + 1);
                printf("  ");
            }
        }
    }
}

void KeyControl(Piece* p){
    if (_kbhit()){
        switch (_getch()){
            case 'a':case 'A': p->x--;break;
            case 'd':case 'D': p->x++;break;
            case 's':case 'S': p->y++;break;
        }
    }
}

void LockPieceToMap(Piece* p){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (p->shape[i][j] != 0){
                int mapX = p->x + j;
                int mapY = p->y + i;
                if (mapY >= 0 && mapY <= HEIGHT && mapX >= 0 && mapX <= WIDTH){
                    map[mapY][mapX] = 1;
                }
            }
        }
    }
}

int CheckColloction(Piece* p, int newX, int newY){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (p->shape[i][j] != 0){
                int mapX = newX + j;
                int mapY = newY + i;
                 
                if (mapX < 0 || mapX >= WIDTH || mapY >= HEIGHT){
                    return 1;
                }
                if (mapY > 0 && map[mapY][mapX] != 0){
                    return 1;
                }
            }
        }
    }
    return 0;
}

void RotatePiece(Piece* p){
    int temp[4][4]={0};
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            temp[j][3-i] = p->shape[i][j];
        }
    }
    Piece text = *p;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            text.shape[i][j] = temp[i][j];
        }
    }
    if (!CheckColloction(&text,p->x,p->y)){
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                p->shape[i][j]=temp[i][j];
            }
        }
    }
}

int ClearFullLines(){
    int linesCleared=0;
    for (int i = HEIGHT - 1; i >= 0; i--){
        int isFull = 1;
        for (int j = 0; j < WIDTH ; j++){
            if (map[i][j] == 0){
                isFull=0;
                break;
            }
        }
        if (isFull){
            linesCleared++;
            score+=10;
            for (int k = i; k > 0; k--){
                for (int j = 0; j < WIDTH; j++){
                    map[k][j]=map[k-1][j];
                }
            }
            for (int j = 0; j < WIDTH; j++){
                map[0][j] = 0;
            }
            i++;
        }
    }
    return linesCleared;
}

void GameRun(){
    Piece p;
    GeneratePiece(&p);

    int timer = 0;

    while (1){
        if (_kbhit()){
            switch (_getch()){
                case 'w':case 'W':
                RotatePiece(&p);
                break;
                case 'a':case 'A':
                if (!CheckColloction(&p, p.x - 1, p.y)) p.x--;
                break;
                case 'd':case 'D':
                if (!CheckColloction(&p, p.x + 1, p.y)) p.x++;
                break;
                case 's':case 'S':
                if (!CheckColloction(&p, p.x, p.y + 1)) p.y++;
                break;
            }
        }

    timer++;
    if (timer>=3) {
        timer = 0;

        if (CheckColloction(&p,p.x,p.y + 1)){
            LockPieceToMap(&p);
            ClearFullLines();
            GeneratePiece(&p);
            if (CheckColloction(&p,p.x,p.y)){
                SetPos(5,10);
                printf("GAME OVER!");
                SetPos(4,11);
                printf("Final Score: %d",score);
                SetPos(1,13);
                printf("Press any key to exit...");
                break;
            }
        }else{
                p.y++;
                }
        DrawScore();
        DrawMap();
        DrawPiece(&p);
        }
    Sleep(50);
    }
}
