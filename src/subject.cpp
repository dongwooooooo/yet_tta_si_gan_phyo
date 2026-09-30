#include "subject.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//검색창에서 연관검색어에 해당하는 과목 이름들 정적으로 선언
const char* autocompleteSubjects[MAX_SUBJECTS] = {
    "데이터통신", "데이터구조", "선형대수학", "프로그래밍설계", "프로그래밍언어", "기초전자", "채플", "웹프로그래밍"
};
//subject.txt에서 유효한 과목이름 정보를 가져옴
Subject* findSubject(char* searchName) {
    FILE* fp = NULL;
    char line[250];
    char* token, * classtoken;
    fp = fopen("subject.txt", "r");
    if (fp == NULL) {
        printf("fp is NULL!!\n");
        return NULL;
    }
    while (fgets(line, sizeof(line), fp) != NULL) {
        char name[50] = { 0 };
        // line = name /token -> 나머지 line
        strtok_s(line, "/", &token);
        strcpy_s(name, sizeof(name), line);
        // name과 subjectName이 같으면 0 반환
        if (searchName == NULL||strcmp(name, searchName) != 0)
            continue;
        // null -> 다음 / search(subjectSize)
        char* sizeContext = NULL;
        char* classContext = NULL;
        char* cinfoTemp = NULL;
        short subjectSize = 0;
        sizeContext = strtok_s(NULL, "/", &token);
        // str -> int
        subjectSize = atoi(sizeContext);
        // 다음 {}
        classtoken = strtok_s(NULL, "\n", &token);
        if (sizeContext == NULL && subjectSize == NULL && classtoken == NULL)
            return NULL;

        //txt에서 불러온 데이터를 subject 자료형에 저장
        Subject* subject = (Subject*)malloc(sizeof(Subject));
        memset(subject, 0, sizeof(Subject));
        int addSubjectCount = 0;
        memcpy(subject->subjectName, searchName, 50);
        subject->classSize = subjectSize;
        while (classtoken != NULL && addSubjectCount < subjectSize) {
            cinfoTemp = strtok_s(NULL, "{}", &classtoken);
            int day = -1, start = -1, end = -1;
            sscanf_s(cinfoTemp, "%d,%d,%d", &day, &start, &end);
            if (day == -1 || start == -1 || end == -1)
                printf("classInfoContext error\n");
            ClassInfo* classInfo = (ClassInfo*)malloc(sizeof(ClassInfo));
            classInfo->classDay = day;
            classInfo->classStart = start;
            classInfo->classEnd = end;
            subject->classInfo[addSubjectCount] = classInfo;
            addSubjectCount++;
        }
        fclose(fp);
        return subject;
    }

}
// 검색창에서 사용자가 검색한 단어와 autocompleteSubjects에 저장된 과목이름과 일지한 경우 suggestions로 참조
char** getAutocompleteSubjectNames(const char* input, int* resultCount) {
    char** suggestions = (char**)malloc(MAX_SUBJECTS * sizeof(char*));
    *resultCount = 0;
    for (int i = 0; i < MAX_SUBJECTS && autocompleteSubjects[i] != NULL; i++) {
        if (strstr(autocompleteSubjects[i], input) != NULL) {
            suggestions[*resultCount] = strdup(autocompleteSubjects[i]);
            (*resultCount)++;
        }
    }
    return suggestions;
}

void setClassInfoBySubjectsCounter(short* counter, short subjectSize) {
    memset(counter, 0, sizeof(short) * subjectSize);
}
int selectSubject(SubjectList* subList, Subject* subject) {
    short subCount = subList->subjectSize;
    subList->subject[subCount] = subject;
    subList->subjectSize += 1;
    return 1;
}
int deleteSubject(SubjectList* subList, int idx) {
    Subject* sub = subList->subject[idx];
    if (sub == NULL) {
        return 0;
    }
    for (int i = idx; i < subList->subjectSize; i++) {
        subList->subject[i] = subList->subject[i + 1];
    }
    subList->subjectSize--;
    return 1;
}
