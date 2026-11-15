#pragma once

typedef struct Contact {
    char name[20];
    char phone[20];
    struct Contact* next;
} Contact;

void AddContact(Contact** head,const char* name,const char* phone);
void PrintList(Contact* head);
void DeleteContact(Contact** head,const char* name);
void FreeList(Contact** head);
void SaveToFile(Contact* head);
void LoadFromFile(Contact** head);
