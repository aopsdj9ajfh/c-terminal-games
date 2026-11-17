#include "DinasuorGame.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>

int main(){
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    InitGame();

    GameRun();
    
    getchar();
    return 0;
}