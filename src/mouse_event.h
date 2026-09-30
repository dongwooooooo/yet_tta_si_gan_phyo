#pragma once

#ifndef MOU_EVENT_H

#define MOU_EVENT_H

#include <Windows.h>

/***사용자의 consoleInput중 mouse event를 DTO에 저장하는 모듈***/

//마우스 이벤트 타입
typedef enum {
	NONE,
	MOUSE_LEFT_1,
	MOUSE_LEFT_2,
	MOUSE_RIGHT_1,
	MOUSE_,
	MOUSE_WHEEL_UP,
	MOUSE_WHEEL_DOWN,
}MouseEventEnum;
//마우스 DTO
typedef struct {
	MouseEventEnum mEnum;
	int x;
	int y;
}MouseState;

void handleMouseEvent(MOUSE_EVENT_RECORD mouse, MouseState* mEvent);

#endif // !MOU_EVENT_H

