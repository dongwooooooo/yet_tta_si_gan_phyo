#pragma once

#ifndef KEYBOARD_EVENT_H

#define KEYBOARD_EVENT_H

#include <Windows.h>

/***사용자의 consoleInput중 keyboard event를 DTO에 저장하는 모듈***/


//키보드 이벤트 타입
typedef enum {
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_RETURN,
	KEY_ESCAPE,
	KEY_BACK,
	KEY_CHAR,
	KEY_NONE,
	KEY_TAP,
}KeyboardEvent;

KeyboardEvent handleKeyboardEvent(KEY_EVENT_RECORD keyboard, char* inputChar);

#endif // !KEYBOARD_EVENT_H
