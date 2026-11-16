#include "Maze_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int maze[HEIGHT][WIDTH];
int playerX = 1,playerY = 1;
int dx[4] = {0, 0, -2, 2};
int dy[4] = {-2, 2, 0, 0};
int isAutoMode = 0;
int stackX[WIDTH * HEIGHT];
int stackY[WIDTH * HEIGHT];
int top = 0;

//工具函数
void SetPos(int x, int y){
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

//内部函数
int CheckWin(){
    return (playerX == WIDTH - 2 && playerY == HEIGHT - 2);
}

//功能函数
void InitMaze(){
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            maze[i][j] = 1;
        }
    }
}

//随机prim算法生成墙
void GenerateMaze(){
    // 2. 墙的集合（用一维数组模拟列表）
    // 存储格式：墙的坐标 + 墙对面的坐标
    int wallX[WIDTH * HEIGHT * 4];
    int wallY[WIDTH * HEIGHT * 4];
    int wallNX[WIDTH * HEIGHT * 4];
    int wallNY[WIDTH * HEIGHT * 4];
    int wallCount = 0;

    // 从起点 (1,1) 开始打通
    maze[1][1] = 0;

    for (int i = 0; i < 4; i++) {
        int nx = 1 + dx[i];
        int ny = 1 + dy[i];
        // 严格限制在内部，确保外墙(0, WIDTH-1, HEIGHT-1)被排除了
        if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1) {
            wallX[wallCount] = 1;
            wallY[wallCount] = 1;
            wallNX[wallCount] = nx;
            wallNY[wallCount] = ny;
            wallCount++;
        }
    }

    // 3. 开始挖墙
    while (wallCount > 0) {
        // 随机选一面墙
        int r = rand() % wallCount;
        
        int x = wallX[r];
        int y = wallY[r];
        int nx = wallNX[r];
        int ny = wallNY[r];

        // 如果墙对面的格子还是墙，说明可以打通（避免形成环路）
        if (maze[ny][nx] == 1) {
            // 打通中间的墙
            maze[y + (ny - y) / 2][x + (nx - x) / 2] = 0;
            // 打通对面的格子
            maze[ny][nx] = 0;

            // 把新打通格子四周的墙加入集合
            for (int i = 0; i < 4; i++) {
                int nnx = nx + dx[i];
                int nny = ny + dy[i];
                if (nnx > 0 && nnx < WIDTH - 1 && nny > 0 && nny < HEIGHT - 1) {
                    if (maze[nny][nnx] == 1) {
                        wallX[wallCount] = nx;
                        wallY[wallCount] = ny;
                        wallNX[wallCount] = nnx;
                        wallNY[wallCount] = nny;
                        wallCount++;
                    }
                }
            }
        }

        // 移除这面墙（把数组最后一面墙移过来覆盖）
        wallX[r] = wallX[wallCount - 1];
        wallY[r] = wallY[wallCount - 1];
        wallNX[r] = wallNX[wallCount - 1];
        wallNY[r] = wallNY[wallCount - 1];
        wallCount--;
    }
    //设置出口
    maze[HEIGHT - 2][WIDTH - 2] = 0;
}

void DrawMaze(){
    SetPos(0,0);
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (i == playerY && j == playerX) printf("@");
            else if (maze[i][j] == 1) printf("|");
            else printf(" ");
        }
        printf("\n");
    }
}

void ChangeMode(int ch){
    if (ch == 't' || ch == 'T'){
        isAutoMode = !isAutoMode;
        if (isAutoMode){
            top = 0;
            stackX[top] = playerX;
            stackY[top] = playerY;
            top++;
            maze[playerY][playerX] = 2;
        } else {
            // 切换回手动模式时，清理迷宫中所有标记为2的格子
            for (int i = 0; i < HEIGHT; i++){
                for (int j = 0; j < WIDTH; j++){
                    if (maze[i][j] == 2){
                        maze[i][j] = 0;
                    }
                }
            }
            DrawMaze();   // 切换后立即刷新：清掉 AI 走过的痕迹
        }
    }
}

void HandleManualMode(int ch){
    int nx = playerX;
    int ny = playerY;
    switch(ch){
        case 'w': case 'W': ny--; break;
        case 's': case 'S': ny++; break;
        case 'a': case 'A': nx--; break;
        case 'd': case 'D': nx++; break;
        default: return;
        }
    if (nx > 0 && nx < WIDTH && ny > 0 && ny < HEIGHT && maze[ny][nx] == 0){
        playerX = nx;
        playerY = ny;
    }
    DrawMaze();
}

void HandleAutoMode(){
    if (isAutoMode){
        Sleep(50);
        if (top == 0) return;

        int cx = stackX[top - 1];
        int cy = stackY[top - 1];
        int found = 0;
        
        for (int i = 0; i < 4; i++){
            int nx = cx + dx[i]/2;
            int ny = cy + dy[i]/2;

            if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && maze[ny][nx] == 0){
                playerX = nx;
                playerY = ny;
                maze[ny][nx] = 2;
            
                stackX[top] = nx;
                stackY[top] = ny;
                top++;
                found = 1;
                break;
            }
        }
        if (!found){
            // 回溯只回退栈指针，不能把已访问格改回 0，
            // 否则上一格会再次被当成未访问格反复进入，导致在死胡同口振荡、走不出迷宫
            if (top > 1){
                top--;
                playerX = stackX[top - 1];
                playerY = stackY[top - 1];
            } else {
                // 只剩起点且无路可走：迷宫确实没有通路，退出 AI 模式避免原地空转
                isAutoMode = 0;
            }
        }
        DrawMaze();
    }
}

void PrintMode(){
    SetPos(10,22);
    printf("操控状态：");
    isAutoMode? printf(" AI ") : printf(" YOU ");
}

void HideCursor(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(hOut, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ci);
}

void GameRun(){
    HideCursor();
    InitMaze();
    GenerateMaze();
    DrawMaze();
    while (1){
        PrintMode();
        if (CheckWin()){
            printf("\n\nYou win! 你走出了迷宫");
            break;
        }
        if (_kbhit()){
            int ch = _getch();
            ChangeMode(ch);
            if (!isAutoMode) HandleManualMode(ch);
        }
        if (isAutoMode) {
            HandleAutoMode();
        }
    }
}