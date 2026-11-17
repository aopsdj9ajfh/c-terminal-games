#include "system.h"
#include <stdio.h>
#include <string.h>

/// @brief 
Student students[MAX_STUDENTS];
int studentCount = 0;

void AddStudent(){
    if (studentCount >= MAX_STUDENTS){
        printf("系统已满，无法添加！");
        return;
    }
    Student *s = &students[studentCount];
    printf("请输入学号");
    scanf("%d", &s->id);
    printf("请输入姓名：");
    scanf("%s", s->name);
    printf("请输入成绩-语文-数学-英语：");
    scanf("%d %d %d", &s->score.Chinese, &s->score.Math, &s->score.English);
    s->score.Total = s->score.Chinese + s->score.Math + s->score.English;

    studentCount++;
    printf("添加成功！");
}

void PrintStudents(){
    printf("\n%-10s %-10s %-8s %-8s %-8s %8s\n", "学号", "姓名", "语文", "数学", "英语", "总分");
    for (int i = 0; i < studentCount; i++){
        Student s = students[i];
        printf("\n%-10d %-10s %-8d %-8d %-8d %-8d", s.id, s.name, s.score.Chinese, s.score.Math, s.score.English, s.score.Total);
    }
}

void SortStudents(){
    Student *ptrs[MAX_STUDENTS];
    for (int i = 0; i < studentCount; i++) ptrs[i] = &students[i];
    for (int i = 0; i < studentCount; i++){
        for (int j = 0; j < studentCount - i - 1; j++){
            if (ptrs[j]->score.Total < ptrs[j+1]->score.Total){
                Student* temp = ptrs[j];
                ptrs[j] = ptrs[j+1];
                ptrs[j+1] = temp;
            }
        }
    }
    printf("\n按总分降序排名\n");
    for (int i = 0; i < studentCount; i++){
        printf("第%d名: %s ,总分: %d \n", i+1, ptrs[i]->name, ptrs[i]->score.Total);
    }
}

void FindStudent(){
    char targetStudentName[20];
    printf("请输入你要查询的学生名字：");
    scanf("%s", targetStudentName);
    for (int i = 0; i < studentCount; i++){
        if (strcmp(students[i].name, targetStudentName) == 0) {
            printf("查到了！学号：%d. 总分：%d\n", students[i].id, students[i].score.Total);
            return;
        }
    }
    printf("未找到该学生！\n");
}

void DeleteStudent(){
    int targetStudentId;
    printf("请输入你要删除学生的学号：");
    scanf("%d",&targetStudentId);
    for (int i = 0; i < studentCount; i++){
        if (students[i].id == targetStudentId){
            for (int j = i; j < studentCount - 1; j++){
                students[j] = students[j+1];
            }
            studentCount--;
            printf("删除成功");
            return;
        }
    }
    printf("没找到您要删除的学生");
}