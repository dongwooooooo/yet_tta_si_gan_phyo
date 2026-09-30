#include "console_input.h"
#include "screen_search_handler.h"
#include "screen_select_schedule_handler.h"
#include "subject.h"
#include <stdarg.h>

// 유저이벤트 ENABLE_MOUSE_INPUT setting
void setConsoleInputMode() {
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hInput, &mode);
	mode |= ENABLE_MOUSE_INPUT; //| ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
	SetConsoleMode(hInput, mode);
}
//가변 인수로 DTO 전달. 
//console event 읽어오는 함수
void readConsoleInput(ServiceState* serviceState, int args, ...) {
	va_list ap;
	SelectedInfo* selInfo = NULL;
	ScheduleGraphInfo* schgrapInfo = NULL;
	va_start(ap, args);
	// searching, modifing은 검색창 creating은 시간표 선택창
	if (serviceState->ssEnum == SEARCHING) {
		selInfo = va_arg(ap, SelectedInfo*);
	}
	if (serviceState->ssEnum == CREATING) {
		schgrapInfo = va_arg(ap, ScheduleGraphInfo*);
	}
	if (serviceState->ssEnum == MODIFING) {
		selInfo = va_arg(ap, SelectedInfo*);
	}
	va_end(ap);

	// console event 자료형, 이를 저장하는 inputRecord 
	setConsoleInputMode();
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD events;
	INPUT_RECORD inputRecord[128];
	//검색창 세팅
	if (serviceState->ssEnum == SEARCHING || serviceState->ssEnum == MODIFING) {
		//검색 단어를 저장하는 SearchStr, 커서 위치정보를 저장하는 CursorPos자료형 선언, 초기화
		SearchStr* istr = (SearchStr*)calloc(1, sizeof(SearchStr));
		CursorPos* cusorPos = (CursorPos*)malloc(sizeof(CursorPos));
		cusorPos->charSizeCounter = 0;
		cusorPos->currentcharSizeCounter = 0;
		cusorPos->cursorPosition = 0;
		istr->ichCount = 0;
		while (1) {
			//Window API를 통해 이벤트 리슨
			ReadConsoleInput(hInput, inputRecord, 128, &events);
			//screen_search_handler모듈 호출
			searchingHandler(inputRecord, istr, events, cusorPos, serviceState, selInfo);
			if (serviceState->ssEnum == STOPSEARCH || serviceState->ssEnum == ESC) {
				break;
			}
		}
		free(istr);
		free(cusorPos);
	}
	//시간표 선택창 세팅
	else if (serviceState->ssEnum == CREATING) { 
		while (1) {
			ReadConsoleInput(hInput, inputRecord, 128, &events);
			//screen_select_schedule_handler모듈 호출
			selectScheduleHandler(inputRecord, events, serviceState, schgrapInfo);
			if (serviceState->ssEnum == STOPCREATING || serviceState->ssEnum == ESC) {
				break;
			}
		}
	}
}