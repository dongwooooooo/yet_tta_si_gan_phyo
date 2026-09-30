#pragma once

#ifndef CURSOR_H
#define CURSOR_H
#include <Windows.h>

/*커서의 위치설정, 위치정보, 커서상태 설정*/

void setCursor(int x, int y);
COORD getCursor(void);
void showCursor(int show);
void setConsoleColor(WORD color);
#endif // CURSOR_H
