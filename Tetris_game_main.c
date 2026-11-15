#include "Tetris_game.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int main(){
    SetConsoleOutputCP(65001);
    srand((unsigned)time(NULL));

    InitGame();

    DrawMap();

    GameRun();
    getchar();
    return 0;
}