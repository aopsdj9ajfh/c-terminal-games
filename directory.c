#include "directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//添加头节点
void AddContact(Contact** head,const char* name,const char* phone){
    //向堆区申请一块内存
    Contact* newNode = (Contact*) malloc(sizeof(Contact));
    if (newNode == NULL){
        printf("内存分配失败");
        return;
    }
    //填充数据
    strcpy(newNode->name,name);
    strcpy(newNode->phone,phone);
    //指针勾连
    newNode -> next = *head;
    *head = newNode;
}

void PrintList(Contact* head){
    Contact* curr=head; //临时滑动指针,防止头指针丢失
    printf("\n---通讯录列表---\n");
    while (curr != NULL){
        printf("姓名：%s,电话：%s\n",curr->name,curr->phone);
        curr=curr->next; //指针后移
    }
    printf("---------------\n");
}

//删除节点（双指针短链法）
void DeleteContact(Contact** head,const char* name){
    Contact* curr = *head;
    Contact* prev = NULL;
    
    while (curr != NULL){
        if (strcmp(curr->name,name)==0){
            Contact* toDelete = curr;
            if (prev==NULL){
                *head = curr->next;
                curr = *head;
            }else{
                prev->next=curr->next;
                curr=prev->next;
            }
            free(toDelete);
        }else{
            prev=curr;
            curr=curr->next;
        }
    }
}
//释放整个链表内存
void FreeList(Contact** head){
    Contact* curr = *head;
    Contact* nextNode = NULL;
    while (curr != NULL){
        nextNode = curr -> next;
        free(curr);
        curr = nextNode;
    }
    *head = NULL;
    printf("所有内存均已释放完毕。\n");
}

void SaveToFile(Contact* head){
    FILE* fp = fopen("Contacts.txt","w");
    if (fp == NULL){
        printf("文件打开失败");
        return;
    }
    Contact* curr = head;
    while (curr != NULL){
        fprintf(fp,"%s %s\n",curr -> name,curr -> phone);
        curr = curr -> next;
    }
    fclose(fp);
    printf("文件已保存到本地！\n");
}

void LoadFromFile(Contact** head){
    FILE* fp = fopen("Contacts.txt","r");
    if (fp == NULL){
        return;  
    }
    char name[20];
    char phone[20];
    while (fscanf(fp,"%s %s",name,phone)==2){
        AddContact(head,name,phone);
    }
    fclose(fp);
    printf("已从本地加载历史数据");
}