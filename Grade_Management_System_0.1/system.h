#pragma once

#define MAX_STUDENTS 100

typedef struct {
    int Chinese;
    int Math;
    int English;
    int Total;
} Score;

typedef struct {
    int id;
    char name[20];
    Score score;
} Student;

extern Student students[MAX_STUDENTS];
extern int studentCount;

void AddStudent();
void PrintStudents();
void SortStudents();
void FindStudent();
void DeleteStudent();