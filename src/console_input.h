#pragma once
#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H
#include "schedule.h"
#include "subject.h"

/***사용자의 consoleInput을 받아오는 window API함수를 정의하는 모듈이다.***/

//검색창에서 한 글자에 대한 자료형
typedef struct inputChar {
	char ich[4];
}InputChar;
//검색창에서 전체 글자에 대한 자료형
typedef struct searchStr {
	InputChar ichs[76];
	char* selectStr[10];
	int ichCount = 0;
}SearchStr;
//서비스 타입
typedef enum {
	SEARCHING,
	STOPSEARCH,
	CREATING,
	STOPCREATING,
	MODIFING,
	LOADING,
	STOPLOADING,
	ESC,
}SSEnum;
//서비스 내에서 사용되는 DTO
typedef struct serviceState {
	SSEnum ssEnum;
	SubjectList* subjectList;
	ScheduleList* scheduleList;
	int selectIdx;
	Schedule* selectedSchedule;
}ServiceState;
//cursor 위치정보
typedef struct cursorPos {
	int charSizeCounter = 0;
	int cursorPosition = 0;
	int currentcharSizeCounter = 0;
}CursorPos;
void setConsoleInputMode();
void readConsoleInput(ServiceState* serviceState, int args, ...);
#endif // !CONSOLE_INPUT_H