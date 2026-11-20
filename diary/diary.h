#pragma once 

#define MAX_STR 100

typedef struct {
    char username[20];
    char userword[20];
} User;

void RegisterUser();
int LoginUser(char* username);
void ReadDiary(const char* username);
void WriteDiary(const char* username);
void DiaryRun();