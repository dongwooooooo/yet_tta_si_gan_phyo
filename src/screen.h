#pragma once
#ifndef SCREEN_H
#define SCREEN_H
#include "console_input.h"
#include "screen_search_handler.h"
#include <Windows.h>

/* 서비스 로직중 사용자의 화면출력을 정의하는 모듈이다
** 정적데이터모듈 static_data와 console_input과 
** screen_handler을 이용한 동적데이터를 처리를 정의하는 모듈
*/

//정적데이터에 대한 길이
constexpr auto SEARCH_WIDTH = 76;
constexpr auto ST_SEARCH_X = 30;
constexpr auto ST_SEARCH_Y = 15;
constexpr auto ST_SUGGEST_X = 30;
constexpr auto ST_SUGGEST_Y = 17;
constexpr auto SUGGEST_WIDTH = 76;
constexpr auto SELECTED_WIDTH = 76;
constexpr auto ST_SELECTED_X = 35;
constexpr auto ST_SELECTED_Y = 35;
constexpr auto ST_BUTTON_X = 108;
constexpr auto ST_BUTTON_Y = 14;
constexpr auto BUTTON_WIDTH = 9;
constexpr auto BUTTON_HEIGHT = 3;

constexpr auto ST_SCHEDULEGRAPH_X = 37;
constexpr auto ST_SCHEDULEGRAPH_Y = 1;
constexpr auto ST_PREV_SCHEDULEBUTTON_X = 20;
constexpr auto ST_PREV_SCHEDULEBUTTON_Y = 15;
constexpr auto ST_NEXT_SCHEDULEBUTTON_X = 110;
constexpr auto ST_NEXT_SCHEDULEBUTTON_Y = 15;
constexpr auto ST_SELECT_SCHEDULEBUTTON_X = 60;
constexpr auto ST_SELECT_SCHEDULEBUTTON_Y = 34;


int titleScreen(void);
int myScheduleScreen(void);
void searchBox(ServiceState* serviceState);
bool searchButtonStage(int x, int y);
bool selectedStage(int x, int y);
void scheduleGraph(ServiceState* serviceState);
void sendKey(WORD vk);
void setSelectedList(ServiceState* serviceState, SelectedInfo* selectedInfo);
void printMySchedule(ServiceState* serviceState);
#endif // !SCREEN_H
