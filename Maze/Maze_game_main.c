#include "Maze_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

int main(){
    SetConsoleOutputCP(65001);
    srand((unsigned int)time(NULL));

    GameRun();

    getchar();
    return 0;
}