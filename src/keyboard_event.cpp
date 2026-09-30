#include "keyboard_event.h"

KeyboardEvent handleKeyboardEvent(KEY_EVENT_RECORD keyboard, char* inputChar) {
    // 키보드 이벤트 처리 로직
    if (keyboard.bKeyDown) {
        switch (keyboard.wVirtualKeyCode) {
        case VK_UP:
            return KEY_UP;
        case VK_DOWN:
            return KEY_DOWN;
        case VK_LEFT:
            return KEY_LEFT;
        case VK_RIGHT:
            return KEY_RIGHT;
        case VK_RETURN:
            return KEY_RETURN;
        case VK_ESCAPE:
            return KEY_ESCAPE;
        case VK_BACK:
            return KEY_BACK;
        case VK_TAB:
            return KEY_TAP;
        default:
            //한글 설정을 위해서 unicode로 인코딩
            if (keyboard.uChar.UnicodeChar != 0) {
                //window API를 통해 한국어와 같은 조합문자를 설정해둔 코드페이지 중 949에 해당하는 한국어 코드페이지 사용
                //한국어 utf-8 = 3바이트, 16=2바이트, 종료문자 널포함 최대 4바이트 세팅
                int len = WideCharToMultiByte(949, 0, &keyboard.uChar.UnicodeChar, 1, inputChar, 4, NULL, NULL);
                inputChar[len] = '\0'; //null- inputchar 종료 
                return KEY_CHAR;
            }
            break;
        }
    }
    return KEY_NONE;
}