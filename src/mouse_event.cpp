#include "mouse_event.h"

void handleMouseEvent(MOUSE_EVENT_RECORD mouse, MouseState* mEvent) {
    
    // 마우스 이벤트 처리 로직
    switch (mouse.dwEventFlags) {
        //이벤트 타입, 이벤트 발생한 마우스 좌표를 저장
    case 0:
        if (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
            mEvent->mEnum = MOUSE_LEFT_1;
            mEvent->x = mouse.dwMousePosition.X;
            mEvent->y = mouse.dwMousePosition.Y;
        }
        if (mouse.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) {
            mEvent->mEnum = MOUSE_LEFT_2;
            mEvent->x = mouse.dwMousePosition.X;
            mEvent->y = mouse.dwMousePosition.Y;
        }
        if (mouse.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
            mEvent->mEnum = MOUSE_RIGHT_1;
            mEvent->x = mouse.dwMousePosition.X;
            mEvent->y = mouse.dwMousePosition.Y;
        }
    default:
        break;
    case MOUSE_WHEELED:
        if ((short)HIWORD(mouse.dwButtonState) > 0) {
            mEvent->mEnum = MOUSE_WHEEL_UP;
            mEvent->x = mouse.dwMousePosition.X;
            mEvent->y = mouse.dwMousePosition.Y;
        }
        else {
            mEvent->mEnum = MOUSE_WHEEL_DOWN;
            mEvent->x = mouse.dwMousePosition.X;
            mEvent->y = mouse.dwMousePosition.Y;
        }
        break;
    }
}


