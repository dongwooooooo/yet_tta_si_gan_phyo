#include "static_data.h"
#include "screen.h"
#include "screen_search_handler.h"
#include "screen_select_schedule_handler.h"
#include "cursor.h"
#include "subject.h"
#include "console_input.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
//메인화면 스크린
int titleScreen(void) {
	int selectService;
	char c;
	printTitleScreen(24, 10);
	showCursor(0);
	while (1) {
		c = _getch();
		if (c >= '1' && c <= '4') {
			selectService = c - '0';
			break;
		}
		else {
			printf(">_<");
		}
	}
	return selectService;
}
//시간표 확인 스크린
int myScheduleScreen(void) {
	int selectService;
	char c;
	showCursor(0);
	while (1) {
		c = _getch();
		if (c == 27) {
			selectService = 0;
			break;
		}
		else {
			printf(">_<");
		}
	}
	return selectService;
}
//시간표 생성에서 기존에 선택했던 시간표를 selectedList에 시간표 이름을 저장
void setSelectedList(ServiceState* serviceState, SelectedInfo* selectedInfo) {
	for (int k = 0; k < serviceState->subjectList->subjectSize; k++) {
		int len = strlen(serviceState->subjectList->subject[k]->subjectName);
		selectedInfo->totalSubNameLen += len+1;
		selectedInfo->subjectNameLen[k] = len;
		selectedInfo->selectCount++;
		if (selectedInfo->totalSubNameLen <= SELECTED_WIDTH) {
			selectedInfo->nextPageIdx = selectedInfo->selectCount;
			selectedInfo->currentSubNameLen += len + 1;
		}
	}
	
	updateSelectedList(serviceState, selectedInfo);
}
//검색화면 정적데이터 print, 동적데이터 readConsoleInput 지정
void searchBox(ServiceState* serviceState) {
	system("cls");
	system("mode con: cols=200 lines=50");
	//정적데이터
	printSearchBox(ST_SEARCH_X, ST_SEARCH_Y);
	printSearchButton(ST_BUTTON_X, ST_BUTTON_Y);
	printSelectedBox(ST_SELECTED_X, ST_SELECTED_Y);
	//동적데이터
	//선택한 과목 저장하는 DTO설정
	SelectedInfo* selInfo = (SelectedInfo*)calloc(1, sizeof(SelectedInfo));
	if (serviceState->ssEnum == MODIFING) {
		//시간표 수정은 기존 선택했던 과목들 정보를 DTO에 저장
		setSelectedList(serviceState, selInfo);
	}
	readConsoleInput(serviceState, 1, selInfo);
	free(selInfo);
}
//시간표 선택화면 정적데이터 print, 동적데이터 readConsoleInput 지정
void scheduleGraph(ServiceState* serviceState) {
	system("cls");
	//정적데이터
	printScheduleGraph(ST_SCHEDULEGRAPH_X, ST_SCHEDULEGRAPH_Y);
	printSelectAcceptButton(ST_SELECT_SCHEDULEBUTTON_X, ST_SELECT_SCHEDULEBUTTON_Y);
	printSelectRightButton(ST_NEXT_SCHEDULEBUTTON_X, ST_PREV_SCHEDULEBUTTON_Y);
	updateSchedule(serviceState->scheduleList->schedule[0]);
	//동적데이터
	//선택한 과목 저장하는 DTO설정
	ScheduleGraphInfo* schgrapInfo = (ScheduleGraphInfo*)calloc(1, sizeof(ScheduleGraphInfo));
	schgrapInfo->page = 0;
	schgrapInfo->size = serviceState->scheduleList->scheduleListSize;
	schgrapInfo->sort = true;
	readConsoleInput(serviceState, 1, schgrapInfo);
	free(schgrapInfo);
}
//선택한 시간표 출력 화면
void printMySchedule(ServiceState* serviceState) {
	system("cls");
	printMyScheduleGraph(ST_SCHEDULEGRAPH_X, ST_SCHEDULEGRAPH_Y);
	updateSchedule(serviceState->selectedSchedule);
}
//마우스 클릭 좌표 범위 설정
bool searchButtonStage(int x, int y) {
	if (x >= ST_BUTTON_X && x <= ST_BUTTON_X + BUTTON_WIDTH && y >= ST_BUTTON_Y && y < ST_BUTTON_Y+BUTTON_HEIGHT) {
		return true;
	}
	return false;
}
bool selectedStage(int x, int y) {
	if ((x >= ST_SELECTED_X) && (x < ST_SELECTED_X + SELECTED_WIDTH) && (y == ST_SELECTED_Y)) {
		return true;
	}
	return false;
}
//console event발생시키는 함수
void sendKey(WORD vk) {
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	//press
	input.ki.wVk = vk;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));
	//release
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}