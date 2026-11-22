#define  _CRT_SECURE_NO_WARNINGS 1
#include "put_box.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

char map[ROW][COM] = {
    "############",
    "#     #    #",
    "#  $  # +  #",
    "#     #    #",
    "#  ####    #",
    "#  @       #",
    "#          #",
    "#    $  +  #",
    "#    $  +  #",
    "############"
};
char tmp[ROW][COM];
int px, py;

static void SaveInitialMap(void)
{
    memcpy(tmp, map, sizeof(map));
}

void ShowMap()
{
    system("cls");
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COM; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    printf("w上 a左 s下 d右 | r重置 | q退出\n");
}

void FindPlayer()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COM; j++)
        {
            if (map[i][j] == '@')
            {
                px = i; py = j;
                return;
            }
        }
    }
}

int CheckWin()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COM; j++)
        {
            if (map[i][j] == '+')
                return 0;
        }
    }
    return 1;
}

static void PlayerLeave()
{
    if (map[px][py] == '@')
        map[px][py] = ' ';
    else if (map[px][py] == '.')
        map[px][py] = '+';
}

static void PlayerEnter(int x, int y){
    if (map[x][y] == '+') map[x][y] = '.';
    else map[x][y] = '@';
    px = x; 
    py = y;
}

void Move(int dx, int dy){
    int nx = px + dx;
    int ny = py + dy;
    if (map[nx][ny] == '#') return;
    if (map[nx][ny] == ' ' || map[nx][ny] == '+') {PlayerLeave(); PlayerEnter(nx, ny);}
    if (map[nx][ny] == '$' || map[nx][ny] == '*') {
        int bx = nx + dx; int by = ny + dy;
        if (map[bx][by] == '#' || map[bx][by] == '*' || map[bx][by] == '$'){
            return;
        }
        PlayerLeave();
        if (map[bx][by] == '+') map[bx][by] = '*';
        else map[bx][by] = '$';
        PlayerEnter(nx, ny);
    }
}

void Reset(){
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COM; j++)
            map[i][j] = tmp[i][j];
    FindPlayer();
}

void GameRun(){
    char ch;
    SaveInitialMap();
    FindPlayer();
    while (1){
        ShowMap();
        if (CheckWin()) {printf("🎉通关了！"); getchar(); break;}
        int ch = _getch();
        switch (ch){
            case 'W':case 'w': Move(-1, 0); break;
            case 'S':case 's': Move(1, 0);  break;
            case 'A':case 'a': Move(0, -1); break;
            case 'D':case 'd': Move(0, 1);  break;
            case 'R':case 'r': Reset();     break;
            case 'Q':case 'q': return;      break;
        }
    }
}