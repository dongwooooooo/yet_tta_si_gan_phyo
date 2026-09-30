#pragma once
#ifndef SCREEN_SELECT_SCHEDULE_HANDLER_H
#define SCREEN_SELECT_SCHEDULE_HANDLER_H

#include "console_input.h"

/* screen_handler을 이용한 동적데이터를 처리를 정의하는 모듈
** 시간표 선택창에 대한 동적데이터 처리를 정의한다.*/

//시간표 출력 정보를 저장하는 자료형
typedef struct scheduleGraphInfo {
	int page;
	int size;
	bool sort;
}ScheduleGraphInfo;
void selectScheduleHandler(INPUT_RECORD* inputRecord, DWORD events, ServiceState* serviceState, ScheduleGraphInfo* schgrapInfo);
void updateSchedule(Schedule* schedule);
void printScheduleInfo(int day, int stGyosi, int finGyosi, char* subName);
void printClsSchedule(Schedule* schedule);
void clsScheduleInfo(int day, int stGyosi, int finGyosi, char* subName);


#endif // !SCREEN_SELECT_SCHEDULE_HANDLER_H
