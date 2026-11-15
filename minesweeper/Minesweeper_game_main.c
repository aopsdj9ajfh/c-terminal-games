#include "Minesweeper_game.h"
#include <stdlib.h>
#include <time.h>

int main(){
    srand((unsigned int) time (NULL));

    char mine[ROWS][COLS]={0};
    char show[ROWS][COLS]={0};
    
    InitBoard(mine,ROWS,COLS,'0');
    InitBoard(show,ROWS,COLS,'*');

    SetMine(mine,ROWS,COLS);

    FindMine(mine,show,ROWS,COLS);

    return 0;
}
