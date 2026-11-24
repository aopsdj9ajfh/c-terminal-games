#include "bird.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

Bird bird;
Block block[50];
Mouse mouse;
int blockCnt = 0;
int score = 0;

//工具函数
void SetPos(int x, int y){
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void HideCursor(){
    CONSOLE_CURSOR_INFO pos = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pos);
}

void DisableQuickEditMode(){
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(hInput, &mode)) {
        mode &= ~ENABLE_QUICK_EDIT_MODE;
        mode |= ENABLE_EXTENDED_FLAGS;
        SetConsoleMode(hInput, mode);
    }
}

void GetMouse(Mouse* m){
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(GetConsoleWindow(), &pt);
    // 安全转换：像素 → 字符坐标
    // 大多数控制台默认字体 8×16 或 8×18，取保守值
    int charW = 8;
    int charH = 16;
    
    // 防止除零
    if (charW <= 0) charW = 8;
    if (charH <= 0) charH = 16;
    
    m->ex = pt.x / charW;
    m->ey = pt.y / charH;
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        if (!m->down) {
            m->down = 1;
            m->sx = pt.x;
            m->sy = pt.y;
        }
    }else{
        if (m->down) {
            m->down = 0;
            if (!bird.fly) {
                float dx = bird.x - m->ex;
                float dy = bird.y - m->ey;
                bird.fly = 1;
                bird.vx = dx * SPEED_K;
                bird.vy = dy * SPEED_K;
            }
        }
    }
}

//主功能函数
void InitBird(){
    bird.x = 8;
    bird.y = 14;
    bird.vx = bird.vy = 0;
    bird.fly = 0;
}
void InitBlock(){
    blockCnt = 0;
    for (int i = 10; i <= 12; i++){
        for (int j = 55; j <= 60; j++){
            block[blockCnt++] = (Block) {j, i ,1};
        }
    }
}
void InitGame(){
    HideCursor();
    DisableQuickEditMode();
    InitBird();
    mouse.down = 0;
    InitBlock();
}

void Draw(){
    SetPos(0,0);
    char buf[SCREEN_H][SCREEN_W + 1];
    //清空画布
    for (int y = 0; y < SCREEN_H; y++){
        for (int x = 0; x < SCREEN_W; x++) buf[y][x] = EMPTY;
        buf[y][SCREEN_W] = '\0';
    }
    //画砖块
    for (int i = 0; i < blockCnt; i++){
        if (block[i].alive) buf[block[i].y][block[i].x] = BLOCK_CHAR;
    }
    //画瞄准线
    if (mouse.down && !bird.fly){
        int x0 = mouse.sx;
        int y0 = mouse.sy;
        int x1 = mouse.ex;
        int y1 = mouse.ey;
        float t;
        for (t = 0; t < 1.0f; t+=0.08f){
            int lx = x0 + (x1 - x0)*t;
            int ly = y0 + (y1 - y0)*t;
            if (lx >= 0 && lx < SCREEN_W && ly >= 0 && ly < SCREEN_H){
                buf[ly][lx] = '-';
            }
        }
        //画预判轨迹线
        float px = bird.x;
        float py = bird.y;
        float pvx = (px - mouse.ex) * SPEED_K;
        float pvy = (py - mouse.ey) * SPEED_K;
        for (int i = 0; i < 35; i++){
            int tx = (int)px;
            int ty = (int)py;
            if (px >= 0 && px < SCREEN_W && py >= 0 && py < SCREEN_H){
                buf[ty][tx] = '.';
            }
            px += pvx;
            py += pvy;
            pvy += G;
        }
    }
    //画小鸟
    int ix = (int)bird.x;
    int iy = (int)bird.y;
    if (ix >= 0 && ix < SCREEN_W && iy >= 0 && iy < SCREEN_H){
        buf[iy][ix] = BIRD_CHAR;
    }
    for (int i = 0; i < SCREEN_H; i++){
        printf("%s\n", buf[i]);
    }
    printf("【操作】鼠标按住向后拖拽松手发射 | R重置 | ESC退出\n");
    printf("得分: %d", score);
}

void CheckCollision(){
    int bx = (int)(bird.x + 0.5f);
    int by = (int)(bird.y + 0.5f);
    for (int i = 0; i < blockCnt; i++){
        if (block[i].x == bx && block[i].y == by){
            block[i].alive = 0;
            score+=10;
            InitBird();
            break;
        }
    }
    if (bx < 0 || bx >= SCREEN_W || by > SCREEN_H) InitBird();
}

void Update(){
    GetMouse(&mouse);
    if (bird.fly){
        bird.x += bird.vx;
        bird.y += bird.vy;
        bird.vy += G;
        CheckCollision();
        CheckGameWin();
    }
    if (_kbhit()){
        int ch = _getch();
        switch (ch) {
            case 27: exit(0); 
            case 'r':case 'R': InitBlock(); InitBird(); break;
        }
    }
}

void CheckGameWin(){
    int aliveBlock = 0;
    for (int i = 0; i < blockCnt; i++){
        if (block[i].alive) aliveBlock++;
    }
    if (aliveBlock == 0) {
        SetPos(0,SCREEN_H + 2);
        printf("=========================\n");
        printf("恭喜胜利！你消灭了所有砖块！\n");
        printf("最终胜利得分：%d\n", score);
        printf("按任意键退出");
        _getch();
        exit(0);
    }
}

void GameRun(){
    InitGame();
    while(1){
        Update();
        Draw();
        Sleep(80);
    }
}