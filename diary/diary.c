#include "diary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void RegisterUser(){
    FILE* fp = fopen("diary.txt","a");
    if (fp == NULL) {
        printf("打开文件失败\n");
        perror("fopen");
        return;
    }

    User newUser;
    printf("请输入要注册的账号名：");
    scanf("%s", newUser.username);
    printf("请设置密码：");
    scanf("%s", newUser.userword);

    fprintf(fp, "%s %s\n", newUser.username, newUser.userword);
    fclose(fp);
    fp = NULL;
    printf("注册成功！请前往登录。\n");
}

int LoginUser(char* username){
    FILE* fp = fopen("diary.txt", "r");
    if (fp == NULL){
        printf("您还没有账号，请前往注册\n");
        perror("fopen");
        return 0;
    } 
    char inputName[20],inputPwd[20];
    printf("请输入登录账号名：");
    scanf("%s", inputName);
    printf("请输入登录密码");
    scanf("%s", inputPwd);
    char fileName[20],filePwd[20];
    while (fscanf(fp, "%s %s", fileName, filePwd) == 2){
        if (strcmp(fileName, inputName) == 0 && strcmp(filePwd, inputPwd) == 0){
            strcpy(username, inputName);
            fclose(fp);
            fp = NULL;
            return 1;
        }
    }
    fclose(fp);
    fp = NULL;
    return 0;
}

void WriteDiary(const char* username){
    char filename[50];
    sprintf(filename, "diary_%s.txt", username);
    FILE* fp = fopen(filename, "a");
    if (fp == NULL){
        perror("fopen");
        printf("日记创建失败");
        return;
    }
    printf("请输入你的日记内容(输入回车结束):");
    getchar();
    char context[200];
    fgets(context, sizeof(context), stdin);
    fprintf(fp, "%s", context);
    fclose(fp);
    fp = NULL;
    printf("日记写入成功\n");
}

void ReadDiary(const char* username){
    char filename[50];
    sprintf(filename, "diary_%s.txt", username);
    FILE* fp = fopen(filename,"r");
    if (fp == NULL){
        perror("fopen");
        return;
    }
    printf("\nb-----你的日记------\n");
    char line[200];
    while(fgets(line, sizeof(line), fp) != NULL){
        printf("%s", line);
    }
    printf("------------------\n");
    fclose(fp);
    fp = NULL;
}

void DiaryRun(){
    int isLoggedIn = 0;
    char username[20] = "";
    int choice;

    while(1){
        if (!isLoggedIn){
            printf("\n----注册页面----\n1.登录账号\n2.注册账号\n0.退出\n");
            printf("请输入您的选择：");
            scanf("%d", &choice);
            switch(choice){
                case 1: if(LoginUser(username)) {isLoggedIn = 1; printf("登录成功，欢迎回来！\n"); break;} else {printf("登录失败!请注册账号后再登录\n");break;}
                case 2: RegisterUser(); break;
                case 0: return;
            }
        }else{
            printf("\n-----欢迎%s------\n", username);
            printf("1.写日记\n2.看日记\n3.退出登录\n5.退出系统\n");
            printf("请选择：");
            scanf("%d", &choice);
            switch(choice){
                case 1: WriteDiary(username); break;
                case 2: ReadDiary(username); break;
                case 3: isLoggedIn = 0; printf("已退出登录"); break;
                case 0: return;
            }
        }
    }
}