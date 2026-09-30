#pragma once
#ifndef SUBJECT_H
#define SUBJECT_H

#define CINFOMAXSIZE 10
#define SUBJECTMAXSIZE 10

/* 시간표 CRUD에 대한 로직을 정의
** 현재 서비스에선 읽기만 한다.
*/


//하나의 과목중 한 개의 분반 정보
typedef struct classInfo {
    short classDay;
    short classStart;
    short classEnd;
}ClassInfo;
//하나의 과목정보
typedef struct subject {
    char subjectName[50];
    ClassInfo* classInfo[CINFOMAXSIZE];
    short classSize;
}Subject;
//선택한 과목들을 저장하는 자료형
typedef struct subjectList {
    Subject* subject[SUBJECTMAXSIZE];
    short subjectSize;
}SubjectList;

#define MAX_SUBJECTS 100

extern const char* autocompleteSubjects[MAX_SUBJECTS];

char** getAutocompleteSubjectNames(const char* subjectName, int* resultCount);
Subject* findSubject(char* subjectName);
void setClassSizeList(short* classSizeList, SubjectList* subList);
void setClassInfoBySubjectsCounter(short* counter, short subjectSize);
int selectSubject(SubjectList* subList, Subject* subject);
int deleteSubject(SubjectList* subList, int idx);
#endif // SUBJECT_H
