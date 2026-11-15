#include "directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(){
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    Contact* head = NULL;
    int choice;
    char name[20];
    char phone[20];
    LoadFromFile(&head);
    while (1){
        printf("----通讯录系统----\n"
                "1.添加联系人\n"
                "2.删除联系人\n"
                "3.查看联系人列表\n"
                "0.退出\n" 
        );
        printf("请输入您的选择：");
        scanf("%d",&choice);
        switch(choice){
            case 1:
            printf("输入姓名：");
            scanf("%s",name);
            printf("输入电话号码：");
            scanf("%s",phone);
            AddContact(&head,name,phone);
            break;
            case 2:
            printf("输入要删除的联系人姓名：");
            scanf("%s",name);
            DeleteContact(&head,name);
            break;
            case 3:
            PrintList(head);
            break;
            case 0:
            SaveToFile(head);
            FreeList(&head);
            return 0;
            default:
            printf("无效指令，请重试!\n");
        }
    }
    return 0;
}