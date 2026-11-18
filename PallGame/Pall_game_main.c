#include "Pall_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

int main(){
    SetConsoleOutputCP(65001);
    srand((unsigned int)time(NULL));

    GameRun();
    
    _getch();
    return 0;
}