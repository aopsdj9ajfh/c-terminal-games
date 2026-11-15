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
int isAutoMode = 0;
typedef struct {
    int x,y;
} Node; //定义用于BFS的节点

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
        int ch = _getch();
        if (ch == 't' || ch == 'T'){
            isAutoMode = !isAutoMode;
            return;
        }
        if (!isAutoMode){
            switch (ch) {
                case 'W': case 'w': if (direction != 2) direction = 1; break;
                case 'S': case 's': if (direction != 1) direction = 2; break;
                case 'A': case 'a': if (direction != 4) direction = 3; break;
                case 'D': case 'd': if (direction != 3) direction = 4; break;
            }
        }
    }
}

void AutoPilot(){
    Node queue[1000];  //定义队列，用数组模拟
    int head = 0,tail = 0;
    
    //2.记录路径父节点，用于回溯
    Node prev[35][25];  //存前驱节点坐标
    bool visited[35][25] = {false};  //初始化标记数组
 
    //将蛇头加入队列   
    queue[tail].x = snakeX[0];
    queue[tail].y = snakeY[0];
    tail++;
    visited[snakeX[0]][snakeY[0]] = true;
    
    //四个方向：上，下，左，右
    int dx[4] = {0,0,-1,1};
    int dy[4] = {-1,1,0,0};

    bool found = false;
    
    //开启BFS搜素
    while (head < tail){
        Node curr = queue[head];
        head++;

        if (curr.x == foodX && curr.y == foodY){
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++){
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];

            if (nx <= 0 || nx >= WIDTH || ny <= 0 || ny >= HEIGHT) continue;
            if (visited[nx][ny]) continue;

            bool isBody = false;
            for (int k = 0; k < snakeLength - 1; k++){
                if (nx == snakeX[k] && ny == snakeY[k]){
                    isBody = true;
                    break;
                }
            }
            if (isBody) continue;

            queue[tail].x = nx;
            queue[tail].y = ny;
            tail++;
            visited[nx][ny] = true;
            prev[nx][ny] = curr;
        }
    }

    if (found){
       Node pathNode = {foodX,foodY};
       while (prev[pathNode.x][pathNode.y].x != snakeX[0] || prev[pathNode.x][pathNode.y].y != snakeY[0]){
           pathNode = prev[pathNode.x][pathNode.y];
       }
       if (pathNode.x == snakeX[0] - 1) direction = 3;  //左
       if (pathNode.x == snakeX[0] + 1) direction = 4;  //右
       if (pathNode.y == snakeY[0] - 1) direction = 1;  //上
       if (pathNode.y == snakeY[0] + 1) direction = 2;  //下
    }else{
        direction = rand()%4 + 1;
    }
}

void GameRun() {
    InitGame();
    DrawMap();
    while (!gameOver) {
        KeyControl();
        if (isAutoMode) AutoPilot();
        MoveSnake();
        EatFood();
        DrawSnake();
        DrawFood();
        DrawScore();
        //显示模式
        SetPos(0,HEIGHT + 2);
        printf("Mode: %s .press 'T' to switch", isAutoMode? "  AI  " : " YOU ");

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
