#include <Windows.h>
#include <stdio.h>
#include "screen_select_schedule_handler.h"
#include "screen.h"
#include "cursor.h"
#include "console_input.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "static_data.h"
//시간표 선택창 핸들러
void selectScheduleHandler(INPUT_RECORD* inputRecord, DWORD events, ServiceState* serviceState, ScheduleGraphInfo* schgrapInfo) {
    showCursor(0);
    for (DWORD i = 0; i < events; i++) {
        if (inputRecord[i].EventType == KEY_EVENT) {
            char ch[4];
            KeyboardEvent keyboardEvent = handleKeyboardEvent(inputRecord[i].Event.KeyEvent, ch);
            if (keyboardEvent == KEY_LEFT) {
                //schgrapInfo.sort == true이면 오밀조밀, flase면 간격이 넓은 시간표
                //true는 idx 0부터 시작
                if (schgrapInfo->sort) {
                    //idx 0이하는 변경없다.
                    if (schgrapInfo->page > 0) {
                        system("cls");
                        //시작점으로 이동하므로 page를 감소시킨다.
                        schgrapInfo->page--;
                        if (schgrapInfo->page != 0) {
                            //leftArrow hide
                            printSelectLeftButton(ST_PREV_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);

                        }
                        //시간표 틀 정적데이터 출력후, 갱신되는 시간표 데이터 출력
                        printSelectAcceptButton(ST_SELECT_SCHEDULEBUTTON_X, ST_SELECT_SCHEDULEBUTTON_Y);
                        printSelectRightButton(ST_NEXT_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                        printScheduleGraph(ST_SCHEDULEGRAPH_X, ST_SCHEDULEGRAPH_Y);
                        updateSchedule(serviceState->scheduleList->schedule[schgrapInfo->page]);
                    }
                }
                //false는 idx 끝부터 정렬,출력한다.
                if (!schgrapInfo->sort) {
                    if (schgrapInfo->page < schgrapInfo->size - 1) {
                        system("cls");
                        //시작점으로 이동하므로 page를 증가시킨다.
                        schgrapInfo->page++;
                        if (schgrapInfo->page != schgrapInfo->size - 1) {
                            //rightArrow hide
                            printSelectLeftButton(ST_PREV_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                        }
                        printSelectAcceptButton(ST_SELECT_SCHEDULEBUTTON_X, ST_SELECT_SCHEDULEBUTTON_Y);
                        printSelectRightButton(ST_NEXT_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                        printScheduleGraph(ST_SCHEDULEGRAPH_X, ST_SCHEDULEGRAPH_Y);
                        updateSchedule(serviceState->scheduleList->schedule[schgrapInfo->page]);
                    }
                }
            }
            //left와 반대로 로직구성
            if (keyboardEvent == KEY_RIGHT) {
                if (schgrapInfo->sort) {
                    if (schgrapInfo->page < schgrapInfo->size-1) {
                        system("cls");
                        schgrapInfo->page++;
                        if (schgrapInfo->page != schgrapInfo->size-1) {
                            //rightArrow hide
                            printSelectRightButton(ST_NEXT_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                        }
                        printSelectAcceptButton(ST_SELECT_SCHEDULEBUTTON_X, ST_SELECT_SCHEDULEBUTTON_Y);
                        printSelectLeftButton(ST_PREV_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                        printScheduleGraph(ST_SCHEDULEGRAPH_X, ST_SCHEDULEGRAPH_Y);
                        updateSchedule(serviceState->scheduleList->schedule[schgrapInfo->page]);
                    }
                }
                if (!schgrapInfo->sort) {
                    if (schgrapInfo->page > 0) {
                        system("cls");
                        schgrapInfo->page--;
                        if (schgrapInfo->page != 0) {
                            //leftArrow hide
                            printSelectRightButton(ST_NEXT_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                        }
                        printSelectAcceptButton(ST_SELECT_SCHEDULEBUTTON_X, ST_SELECT_SCHEDULEBUTTON_Y);
                        printSelectLeftButton(ST_PREV_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                        printScheduleGraph(ST_SCHEDULEGRAPH_X, ST_SCHEDULEGRAPH_Y);
                        updateSchedule(serviceState->scheduleList->schedule[schgrapInfo->page]);
                    }
                }
            }
            //사용자가 선택한 시간표에 해당하는 시간표 정보를 selectedSchedule으로 복사한다.
            if (keyboardEvent == KEY_RETURN) {
                for (int i = 0; i < serviceState->scheduleList->scheduleSize; i++) {
                    serviceState->selectedSchedule->scheduleInfo[i] = (ScheduleInfo*)calloc(1, sizeof(ScheduleInfo));
                    serviceState->selectedSchedule->scheduleInfo[i]->classInfo = (ClassInfo*)calloc(1, sizeof(ClassInfo));

                    strcpy(serviceState->selectedSchedule->scheduleInfo[i]->subjectName,
                        serviceState->scheduleList->schedule[schgrapInfo->page]->scheduleInfo[i]->subjectName);
                    serviceState->selectedSchedule->scheduleInfo[i]->classInfo->classDay
                        = serviceState->scheduleList->schedule[schgrapInfo->page]->scheduleInfo[i]->classInfo->classDay;
                    serviceState->selectedSchedule->scheduleInfo[i]->classInfo->classStart
                        = serviceState->scheduleList->schedule[schgrapInfo->page]->scheduleInfo[i]->classInfo->classStart;
                    serviceState->selectedSchedule->scheduleInfo[i]->classInfo->classEnd
                        = serviceState->scheduleList->schedule[schgrapInfo->page]->scheduleInfo[i]->classInfo->classEnd;
                }
                serviceState->selectedSchedule->scheduleSize = serviceState->scheduleList->scheduleSize;
                //시간표 선택 서비스 종료
                serviceState->ssEnum = STOPCREATING;
                break;
            }
            //시간표 선택 서비스 나가기
            if (keyboardEvent == KEY_ESCAPE) {
                serviceState->ssEnum = ESC;
            }
            //정렬방식 변경
            if (keyboardEvent == KEY_TAP) {
                //true = 오밀조밀 false = 우주
                // sort true에서 false로 변경, 리스트 인덱스 끝부분을 시작점으로 설정
                if (schgrapInfo->sort) {
                    system("cls");
                    schgrapInfo->sort = false;
                    schgrapInfo->page = serviceState->scheduleList->scheduleListSize - 1;
                    schgrapInfo->size = serviceState->scheduleList->scheduleListSize;
                    printSelectAcceptButton(ST_SELECT_SCHEDULEBUTTON_X, ST_SELECT_SCHEDULEBUTTON_Y);
                    printSelectRightButton(ST_NEXT_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                    printScheduleGraph(ST_SCHEDULEGRAPH_X, ST_SCHEDULEGRAPH_Y);
                    updateSchedule(serviceState->scheduleList->schedule[serviceState->scheduleList->scheduleListSize-1]);

                }
                // sort false에서 true로 변경, 리스트 인덱스 첫부분을 시작점으로 설정
                else if (!schgrapInfo->sort) {
                    system("cls");
                    schgrapInfo->sort = true;
                    schgrapInfo->page = 0;
                    schgrapInfo->size = serviceState->scheduleList->scheduleListSize;
                    printSelectAcceptButton(ST_SELECT_SCHEDULEBUTTON_X, ST_SELECT_SCHEDULEBUTTON_Y);
                    printSelectRightButton(ST_NEXT_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
                    printScheduleGraph(ST_SCHEDULEGRAPH_X, ST_SCHEDULEGRAPH_Y);
                    updateSchedule(serviceState->scheduleList->schedule[0]);
                }
            }
        }
    }
}
//schedule에 있는 scheduleInfo들을 내용 출력
void updateSchedule(Schedule* schedule) {
    int stGyosi = 0;
    int finGyosi = 0;
    int day = 0;
    for (int i = 0; i < schedule->scheduleSize; i++) {
        day = schedule->scheduleInfo[i]->classInfo->classDay;
        stGyosi = schedule->scheduleInfo[i]->classInfo->classStart;
        finGyosi = schedule->scheduleInfo[i]->classInfo->classEnd;
        printScheduleInfo(day, stGyosi, finGyosi, schedule->scheduleInfo[i]->subjectName);
    }
}
//scheduleInfo의 시작교시, 끝교시를 시간표 틀의 등차수열에 맞춰서 출력
void printScheduleInfo(int day, int stGyosi, int finGyosi, char* subName) {
    int x = ST_SCHEDULEGRAPH_X + 8 +1 + 11 * day;
    int len = strlen(subName);
    for (int i = stGyosi; i <= finGyosi; i++) {
        int y = ST_SCHEDULEGRAPH_Y + i * 3;
        //시간표 이름을 출력하는 줄의 길이는 10, 넘는 경우 다음 줄에 출력, 최대 20칸
        if (len > 10) {
            char buffer[11] = {};
            strncpy(buffer, subName, 10);
            buffer[10] = '\0';
            setCursor(x, y);
            printf("%s", buffer);
            strncpy(buffer, subName + 10, 10);
            buffer[10] = '\0';
            setCursor(x, y + 1);
            printf("%s", buffer);
        }
        else {
            setCursor(x, y);
            printf("%s", subName);
        }
    }
}
//scheduleInfo의 정보를 이용해 기존 출력 내용들을 지운다
void clsScheduleInfo(int day, int stGyosi, int finGyosi, char* subName) {
    int x = ST_SCHEDULEGRAPH_X + 8 + 11 * day;
    int len = strlen(subName);
    char buffer[11];
    memset(buffer, ' ', 10);
    buffer[10] = '\0';
    for (int i = stGyosi; i <= finGyosi; i++) {
        int y = ST_SCHEDULEGRAPH_Y + i * 3;
        if (len > 10) {
            setCursor(x, y);
            printf("%s", buffer);
            setCursor(x, y + 1);
            printf("%s", buffer);
        }
        else {
            setCursor(x, y);
            printf("%s", buffer);
        }
    }
}

void printClsSchedule(Schedule* schedule) {
    int stGyosi = 0;
    int finGyosi = 0;
    int day = 0;
    for (int i = 0; i < schedule->scheduleSize; i++) {
        day = schedule->scheduleInfo[i]->classInfo->classDay;
        stGyosi = schedule->scheduleInfo[i]->classInfo->classStart;
        finGyosi = schedule->scheduleInfo[i]->classInfo->classEnd;
        clsScheduleInfo(day, stGyosi, finGyosi, schedule->scheduleInfo[i]->subjectName);
    }
}