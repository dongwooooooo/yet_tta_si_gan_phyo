#include "schedule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 시간표 정렬 기준 계산 
// betweenTime: 같은 요일 수업간의 간격
// emptyDay: 수업이 없는 요일의 수
void calClsBetweenTimeAndEmptyDay(Schedule* sch) {
    for (int day = 0; day < 5; day++) {
        int maxEnd = -1;
        int minStart = 10;
        int classTimes[10] = { 0 };

        for (int i = 0; i < sch->scheduleSize; i++) {
            if (sch->scheduleInfo[i]->classInfo->classDay == day) {
                for (int j = sch->scheduleInfo[i]->classInfo->classStart;
                    j <= sch->scheduleInfo[i]->classInfo->classEnd; j++) {
                    classTimes[j] = 1;
                    if (sch->scheduleInfo[i]->classInfo->classStart < minStart) {
                        minStart = sch->scheduleInfo[i]->classInfo->classStart;
                    }
                    if (sch->scheduleInfo[i]->classInfo->classEnd > maxEnd) {
                        maxEnd = sch->scheduleInfo[i]->classInfo->classEnd;
                    }
                }
            }
        }
        if (maxEnd == -1) {
            sch->emptyDay += 1;
            continue;
        }
        int betweenTime = 0;
        for (int t = minStart; t <= maxEnd; t++) {
            if (classTimes[t] == 0) {
                betweenTime++;
            }
        }
        sch->clsBetweenTime += betweenTime;
    }
}
// 메인 로직 중 시간표 생성
ScheduleList* createSchedules(SubjectList* subjectList) {
    //최종적으로 생성되는 시간표들을 저장하는 자료형 생성, 초기화
    ScheduleList* scheduleList = (ScheduleList*)calloc(1, sizeof(ScheduleList));
    short subjectSize = subjectList->subjectSize;
    scheduleList->scheduleSize = subjectSize;
 
    //classInfoBySubjectsCounter의 기준이 될 수업들의 분반 수를 저장
    short classSizeList[sizeof(short) * 10] = {};
    setClassSizeList(classSizeList, subjectList);
    // 시간표 생성에서 모든 경우의 시간표를 카운팅할 카운터
    short* classInfoBySubjectsCounter = (short*)malloc(sizeof(short) * subjectSize);
    memset(classInfoBySubjectsCounter, 0, sizeof(short) * subjectSize);

    while (1) {
        //분반의 요일, 시작 교시, 끝 교시를 저장하는 classInfo를 보관할 리스트
        ClassInfo* cinfoList[10] = { NULL };
        //subejcetSize는 사용자가 선택한 수업들의 개수
        for (short j = 0; j < subjectSize; j++) {
            if (subjectList->subject[j] == NULL) {
                break;
            }
            //반복문에 해당하는 idx 과목에서 classInfoBySubjectsCounter의 idx와 대응되는 값인 classInfo idx를 가져옴
            short classInfoSizeOfSubject = classInfoBySubjectsCounter[j];
            //해당하는 분반정보를 리스트에 보관
            cinfoList[j] = subjectList->subject[j]->classInfo[classInfoSizeOfSubject];
        }
        //분반 리스트에 수업이 중복되면 pass
        if (!checkConflictTimeAtSameDay(cinfoList, subjectSize)) {
            //중복이 없으면 시간표 생성이 가능하므로 schedule로 만들고 scheduleList에 저장
            Schedule* sch = makeSchedule(subjectList, cinfoList);
            addToSchList(scheduleList, sch);
        }
        //카운터 증가
        short idx = subjectSize - 1;
        idx = nextIndexOfSubjectList(classInfoBySubjectsCounter, classSizeList, idx);
        if (idx < 0) {
            break;
        }
    }
    return scheduleList;
}
//subject Info -> schedule Info로 변환
Schedule* makeSchedule(SubjectList* subjectList, ClassInfo* cinfoList[]) {
    //schdule 자료형 생성, 초기화
    Schedule* sch = (Schedule*)calloc(1, sizeof(Schedule));
    short subSize = subjectList->subjectSize;
    int i = 0;
    //선택한 과목수 만큼 scheduleInfo생성
    while (i < subSize)
    {
        //subject의 이름, 특정 분반의 요일, 시작교시, 끝교시를 저장->scheudle자료형에 저장 ==> 하나의 시간표 완성
        ScheduleInfo* scheduleInfo = (ScheduleInfo*)malloc(sizeof(ScheduleInfo));
        scheduleInfo->classInfo = (ClassInfo*)malloc(sizeof(ClassInfo));
        char* subjectName = subjectList->subject[i]->subjectName;
        memcpy(scheduleInfo->subjectName, subjectName, 50);
        scheduleInfo->classInfo->classDay = cinfoList[i]->classDay;
        scheduleInfo->classInfo->classStart = cinfoList[i]->classStart;
        scheduleInfo->classInfo->classEnd = cinfoList[i]->classEnd;

        sch->scheduleInfo[i] = scheduleInfo;
        i++;
        sch->scheduleSize++;
    }
    // 생성된 하나의 시간표에 각 요일의 수업간의 간격, 수업 안 가는 요일을 계산하여 schedule에 저장
    calClsBetweenTimeAndEmptyDay(sch);
    return sch;
}

// 시간표 저장 로직
//한 줄씩 "수업이름,수업요일,수업시작교시,수업끝교시" 형태로 저장
int saveSelectedSchedule(Schedule* schedule) {
    FILE* fp = NULL;
    fp = fopen("schedule.txt", "w");
    for (int i = 0; i < schedule->scheduleSize; i++) {
        char buffer[60] = {};
        sprintf(buffer, "%s,%d,%d,%d\n",
            schedule->scheduleInfo[i]->subjectName,
            schedule->scheduleInfo[i]->classInfo->classDay,
            schedule->scheduleInfo[i]->classInfo->classStart,
            schedule->scheduleInfo[i]->classInfo->classEnd);
        int check = fputs(buffer, fp);
        if (check == -1) {
            return 0;
        }
    }
    fclose(fp);
    return 1;
}
// 시간표 불러오는 로직
//한 줄씩 "수업이름,수업요일,수업시작교시,수업끝교시" 형태로 저장되어 있는 txt파일에서 데이터 파싱
int loadSchedule(Schedule* schedule) {
    FILE* fp = NULL;
    char line[250];
    fp = fopen("schedule.txt", "r");
    if (fp == NULL) {
        return 0;
    }
    int count = 0;
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        line[strcspn(line, "\n")] = 0;

        char name[50] = { NULL };
        char* token, * classtoken;
        // line = name /token -> 나머지 line
        strtok_s(line, ",", &token);
        schedule->scheduleInfo[count] = (ScheduleInfo*)calloc(1, sizeof(ScheduleInfo));
        schedule->scheduleInfo[count]->classInfo = (ClassInfo*)calloc(1, sizeof(ClassInfo));
        strcpy_s(schedule->scheduleInfo[count]->subjectName, sizeof(schedule->scheduleInfo[count]->subjectName), line);        short day = 0;
        short st = 0;
        short fin = 0;
        classtoken = strtok_s(NULL, ",", &token);
        day = atoi(classtoken);
        classtoken = strtok_s(NULL, ",", &token);
        st = atoi(classtoken);
        classtoken = strtok_s(NULL, ",", &token);
        fin = atoi(classtoken);
        
        schedule->scheduleInfo[count]->classInfo->classDay = day;
        schedule->scheduleInfo[count]->classInfo->classStart = st;
        schedule->scheduleInfo[count]->classInfo->classEnd = fin;
        count++;
    }
    fclose(fp);
    schedule->scheduleSize = count;
    schedule->scheduleInfo[schedule->scheduleSize] = NULL;
    return 1;
}

//생성된 시간표를 scheduleList에 저장
//학교 안 가는 날이 많고, 같은 날 수업간의 간격이 좁으면 리스트 앞쪽으로 정렬
void addToSchList(ScheduleList* schList, Schedule* sch) {
    int peek = schList->scheduleListSize;
    for (int i = 0; i < schList->scheduleListSize; i++) {
        if (schList->schedule[i]->emptyDay < sch->emptyDay) {
            continue;
        }
        if (schList->schedule[i]->emptyDay == sch->emptyDay) {
            if (schList->schedule[i]->clsBetweenTime < sch->clsBetweenTime) {
                continue;
            }
        }
        else {
            for (int j = schList->scheduleListSize; j > i; j--) {
                schList->schedule[j] = schList->schedule[j - 1];
            }
            peek = i;
            break;
        } 
    }
    schList->schedule[peek] = sch;
    schList->scheduleListSize++;
}
//리스트 오른쪽에서 왼쪽방향으로 카운트 증가
// 만약 classSizeList : {1,1,2}
// counter : 000->001->002->010->011->012->100......
static int nextIndexOfSubjectList(short classInfoBySubjectsCounter[], short classSizeList[], int index) {
    while (index >= 0) {
        classInfoBySubjectsCounter[index]++;
        if (classInfoBySubjectsCounter[index] >= classSizeList[index]) {
            classInfoBySubjectsCounter[index] = 0;
            index--;
        }
        else {
            break;
        }
    }
    return index;
}
// classInfoList에서 같은 요일 + 같은 수업시간이 없는지 중복 확인
static bool checkConflictTimeAtSameDay(ClassInfo* cinfoList[], short subjectSize) {

    for (int i = 0; i < subjectSize - 1; i++) {
        for (int j = i + 1; j < subjectSize; j++) {
            if (cinfoList[i]->classDay == cinfoList[j]->classDay) {
                int difference = cinfoList[i]->classStart - cinfoList[j]->classStart;
                if (difference == 0) {
                    return true;
                }
                if ((difference < 0) && (cinfoList[i]->classEnd >= cinfoList[j]->classStart)) {
                    return true;
                }
                if ((difference > 0) && (cinfoList[i]->classStart <= cinfoList[j]->classEnd)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void setClassSizeList(short* classSizeList, SubjectList* subList) {
    for (int i = 0; i < subList->subjectSize; i++) {
        classSizeList[i] = subList->subject[i]->classSize;
    }
}