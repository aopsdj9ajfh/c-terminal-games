#include "system.h"
#include <stdio.h>
#include <windows.h>

int main(){
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    int choice;
    while (1){
        printf("\n---学生成绩管理系统---\n");
        printf("1.添加学生。\n2.打印列表。\n3.按总分排名。\n4.查询学生。\n5.删除学生\n0.退出\n");
        printf("请输入你的选择：");
        scanf("%d", &choice);
        switch (choice){
            case 1: AddStudent(); break;
            case 2: PrintStudents(); break;
            case 3: SortStudents(); break;
            case 4: FindStudent(); break;
            case 5: DeleteStudent(); break;
            case 0: return 0;
            default: printf("无效输入！\n");
        }
    }
}