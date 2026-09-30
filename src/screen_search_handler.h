#pragma once

#ifndef SCREEN_HANDLER_H

#define SCREEN_HANDLER_H

#include <Windows.h>
#include "console_input.h"

#define MAX_SUGGEST 5


/* screen_handler을 이용한 동적데이터를 처리를 정의하는 모듈
** 검색창에 대한 동적데이터 처리를 정의한다.*/

//선택 과목리스트 정보를 저장하는 자료형
typedef struct selectedInfo {
	int subjectNameLen[10];
	int totalSubNameLen;
	int selectCount;
	int stPageIdx;
	int nextPageIdx;
	int currentSubNameLen;
}SelectedInfo;
void updateSelectedList(ServiceState* serviceState, SelectedInfo* selectedInfo);
void searchingHandler(INPUT_RECORD* inputRecord, SearchStr* istr, DWORD events, CursorPos* cusorPos, 
						ServiceState* serviceState, SelectedInfo* selectedInfo);
void updateAndCheckSelectedList(ServiceState* serviceState, SelectedInfo* selectedInfo, int changeIdx);
void resetCounters(SearchStr* istr, CursorPos* cursorPos);
void printBuffer(char buffer[]);
void updateCursor(CursorPos* cursorPos);
void clsSearchBox(void);
void sendKey(WORD vk);
void clsSelectedBox(void);
void printSelectedBuffer(char selected[]);
void printSuggest(char** suggest, int resultCount);
void clsSuggest(void);


#endif // !SCREEN_HANDLER_H


