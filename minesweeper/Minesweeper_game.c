#include "Minesweeper_game.h"
#include <stdio.h>
#include <stdlib.h>

void InitBoard(char board[ROWS][COLS], int rows, int cols, char set){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            board[i][j] = set;
        }
    }
}

void DisplayBoard(char board[ROWS][COLS], int row, int col){
    printf("  ");
    for (int i = 1; i <= col; i++){
        printf("%d ",i);
    }
    printf("\n");
    for (int i = 1; i <= row; i++){
        printf("%d ",i);
        for (int j = 1; j <= col; j++){
            printf("%c ",board[i][j]);
        }
        printf("\n");
    }
}

void SetMine(char mine[ROWS][COLS], int row, int col){
    int count = 0;
    while (count < MINE_COUNT){
        int x = rand() % row + 1;
        int y = rand() % col + 1;
        if (mine[x][y] == '0'){
            mine[x][y] = '1';
            count++;
        }
    }
}

int GetMineCount(char mine[ROWS][COLS], int x, int y){
    return mine[x-1][y-1] + mine[x-1][y] + mine[x-1][y+1] +
           mine[x][y-1] + mine[x][y+1] +
           mine[x+1][y-1] + mine[x+1][y] + mine[x+1][y+1] - 8 * '0';
}

void Expand(char mine[ROWS][COLS], char show[ROWS][COLS], int x, int y){
    if (x < 1 || x > ROW || y < 1 || y > COL) return;
    int count = GetMineCount(mine, x, y);
    if (count == 0){
        show[x][y] = ' ';
        for (int i = x - 1; i <= x + 1; i++){
            for (int j = y - 1; j <= y + 1; j++){
                if (show[i][j] == '*'){
                    Expand(mine, show, i, j);
                } 
            }
        }
    }else{
        show[x][y] = count + '0';
    }
}

void FindMine(char mine[ROWS][COLS],char show[ROWS][COLS],int row,int col){
    int win=0;
    int x,y;
    while (win < col*row - MINE_COUNT){
        DisplayBoard(show,row,col);
        printf("Please enter (x,y):");
        scanf("%d %d",&x,&y);
        if (x<1 || x>row || y<1 || y>col){
            printf("Invalid coordinates,please enter again:\n");
            continue;
        }
        if (show[x][y] != '*'){
            printf("The location has already been checked\n");
            continue;
        }
        if (mine[x][y] == '1'){
            DisplayBoard(mine,row,col);
            printf("Unfortunately,you steped on a mine\n");
            return;
        }
        Expand(mine,show,x,y);
        win=0;
        for (int i = 1; i <= row; i++){
            for (int j = 1; j <= col; j++){
                if (show[i][j] != '*'){
                win++;
                }
            }
        }
    }
    printf("Congratulations,you cleared the mine!\n");
}
