#pragma once
#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "subject.h"

/*시간표 CRUD에 대한 로직을 정의*/

//시간표에서 하나의 수업에 대한 정보
typedef struct scheduleInfo {
    char subjectName[50];
    ClassInfo* classInfo;
}ScheduleInfo;
//한 종류의 시간표
typedef struct schedule {
    ScheduleInfo* scheduleInfo[10];
    short clsBetweenTime;
    short emptyDay;
    short scheduleSize;
}Schedule;
//생성된 전체 시간표
typedef struct scheduleList {
    Schedule* schedule[300];
    short scheduleSize;
    int scheduleListSize;
}ScheduleList;

ScheduleList* createSchedules(SubjectList* subjectList);
Schedule* makeSchedule(SubjectList* subjectList, ClassInfo* cinfoList[]);
void addToSchList(ScheduleList* schList, Schedule* sch);

static int nextIndexOfSubjectList(short classInfoBySubjectsCounter[], short classSizeList[], int index);
static bool checkConflictTimeAtSameDay(ClassInfo* cinfoList[], short subjectSize);
void setClassSizeList(short* classSizeList, SubjectList* subList);

int saveSelectedSchedule(Schedule* schedule);
int loadSchedule(Schedule* schedule);
#endif // SCHEDULE_H
