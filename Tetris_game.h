#pragma once

#define WIDTH 10
#define HEIGHT 20

extern int map[HEIGHT][WIDTH];
extern int score;

typedef struct {
    int x,y;
    int shape[4][4];
} Piece;

void InitGame();
void DrawMap();
void DrawPiece(Piece* p);
void DrawScore();
void GeneratePiece(Piece* p);
void ClearPiece(Piece* p);
void KeyControl(Piece* p);
void LockPieceToMap(Piece* p);
int CheckColloction(Piece* p, int newX, int newY);
void RotatePiece(Piece* p);
int ClearFullLines();
void GameRun();