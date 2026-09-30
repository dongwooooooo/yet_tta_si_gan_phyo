#pragma once

#ifndef SERVICE_H
#define SERVICE_H

#include "schedule.h"
#include "subject.h"

/* 하나의 프로세스에 대한 비즈니스 로직을 정의한다.
** 메인 화면에 대한 startService
** 시간표 생성 프로세스: createScheduleService
** 시간표 수정 프로세스: modifyScheduleService
** 시간표 확인 프로세스: mySchedule
*/

int startService();
int createScheduleService();
void setConsoleFont();
int modifyScheduleService();
void setConsoleFont();
int mySchedule();

#endif // !SERVICE_H