#include "screen_search_handler.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "console_input.h"
#include "screen.h"
#include "cursor.h"
#include "subject.h"
#include "service.h"
#include <Windows.h>
#include <stdio.h>

//검색창 동적데이터 설정하는 핸들러
//inputRecord : event 저장하는 buffer
//istr: 사용자가 타이핑한 글자들 저장
void searchingHandler(INPUT_RECORD* inputRecord, SearchStr* istr, DWORD events, CursorPos* cursorPos, 
                        ServiceState* serviceState, SelectedInfo* selectedInfo) {
    //updateCursor은 cursorInfo를 통해 검색창의 마지막 타이핑한 단어 옆에 갱신,
    showCursor(0);
    updateCursor(cursorPos);
	for (DWORD i = 0; i < events; i++) {
        //keyboard 이벤트 정의
		if (inputRecord[i].EventType == KEY_EVENT) {
            //입력 단어를 handleKeyboardEvent를 통해 받아옴
            char ch[4] = { 0 };
			KeyboardEvent keyboardEvent = handleKeyboardEvent(inputRecord[i].Event.KeyEvent, ch);
			//키보드 타이핑
            if (keyboardEvent == KEY_CHAR) {
                //검색창 길이 초과하면 패스
				if (istr->ichCount >= SEARCH_WIDTH-1) {
					continue;
				}
				ShowCursor(0);
                //검색어 제안 창 삭제
                clsSuggest();
				char buffer[SEARCH_WIDTH] = {};
				memset(buffer, '\0', SEARCH_WIDTH);
				
				// cursorPosition이 맨 뒤인 상황
				if (istr->ichCount == cursorPos->cursorPosition) {
					//ch를 ichs에 copy한다.
					for (int j = 0; j < 4; j++) {
						istr->ichs[istr->ichCount].ich[j] = ch[j];
					}
					// counters 갱신한다.
					istr->ichCount++;
                    cursorPos->cursorPosition++;
                    //한글은 x길이 2, 그 외 1
					if (ch[1] == 0) {
                        cursorPos->charSizeCounter++;
                        cursorPos->currentcharSizeCounter++;
					}
					else {
                        cursorPos->charSizeCounter += 2;
                        cursorPos->currentcharSizeCounter += 2;
					}
                    //istr을 buffer에 stringCpy한다.
					for (int k = 0; k < istr->ichCount; k++) {
						strcat(buffer, istr->ichs[k].ich);
					}
                    //bufferString의 연관 단어를 set, print
                    int resultCount;
                    char** suggestions;
                    suggestions = getAutocompleteSubjectNames(buffer, &resultCount);
                    printSuggest(suggestions, resultCount);
                    //bufferString print
                    printBuffer(buffer);
                    //cusor를 검색창에 현재 위치로 이동시킨다;
                    updateCursor(cursorPos);
				}

                //커서가 검색어 중간에 위치, 내용 삽입되는 경우
                if (istr->ichCount > cursorPos->cursorPosition) {
                    //삽입 지점 뒤의 글자들을 오른쪽 쉬프트
                    for (int i = istr->ichCount; i >= cursorPos->cursorPosition; i--) {
                        memcpy(istr->ichs[i].ich, istr->ichs[i - 1].ich, 4);
                    }
                    //삽입
                    for (int j = 0; j < 4; j++) {
                        istr->ichs[cursorPos->cursorPosition].ich[j] = ch[j];
                    }
                    //카운터 갱신
                    istr->ichCount++;
                    cursorPos->cursorPosition++;
                    if (ch[1] == 0) {
                        cursorPos->charSizeCounter++;
                        cursorPos->currentcharSizeCounter;
                    }
                    else {
                        cursorPos->charSizeCounter += 2;
                        cursorPos->currentcharSizeCounter += 2;
                    }
                    //istr을 buffer에 stringCpy한다.
                    for (int k = 0; k < istr->ichCount; k++) {
                        strcat(buffer, istr->ichs[k].ich);
                    }
                    //istr에 저장된 글자를 통해 검색 제안 검색, set
                    int resultCount;
                    char** suggestions = NULL;
                    suggestions = getAutocompleteSubjectNames(buffer, &resultCount);
                    // 화면 갱신
                    clsSearchBox();
                    printBuffer(buffer);
                    printSuggest(suggestions, resultCount);
                    updateCursor(cursorPos);
                }
			}
            //keyboard enter
			if (keyboardEvent == KEY_RETURN) {
                showCursor(0);
                clsSearchBox();
                //과목 검색 수 범위 지정
                if (selectedInfo->selectCount < 10) {
                    //istr에 저장된 글자 buffer에 문자열로 저장, findSubject
                    char buffer[SEARCH_WIDTH] = {};
                    memset(buffer, '\0', SEARCH_WIDTH);
                    for (int k = 0; k < istr->ichCount; k++) {
                        strcat(buffer, istr->ichs[k].ich);
                    }
                    Subject* searchedSubject = findSubject(buffer);
                    //검색어가 과목 이름이랑 일치한 경우 로직 진행
                    if (searchedSubject != NULL) {
                        //불러온 과목 데이터를 subjectList에 저장
                        int check = selectSubject(serviceState->subjectList, searchedSubject);
                        if (check == 0) {
                            printf("selectSubject error");
                        }
                        //선택한 과목리스트에 추가되는 과목의 길이, 저장된 전체 과목의 길이, 선택한 과목 수 갱신
                        int size = serviceState->subjectList->subjectSize - 1;
                        selectedInfo->subjectNameLen[size] = cursorPos->charSizeCounter;
                        if (selectedInfo->selectCount == 0) {
                            selectedInfo->totalSubNameLen += cursorPos->charSizeCounter;
                        }
                        else {
                            selectedInfo->totalSubNameLen += cursorPos->charSizeCounter + 1;
                        }
                        selectedInfo->selectCount++;
                        /*선택한 과목이름의 전체 길이가 리스트 길이보다 긴 경우
                        **뒤에서부터 길이 계산, 시작 인덱스 설정*/
                        if (selectedInfo->totalSubNameLen >= SELECTED_WIDTH-1) {
                            int sumOfSubName = 0;
                            int st = 0;
                            int fin = selectedInfo->selectCount;
                            for (int i = fin - 1; i >= 0; i--) {
                                sumOfSubName += selectedInfo->subjectNameLen[i];
                                if (sumOfSubName >= SELECTED_WIDTH-2) {
                                    sumOfSubName -= selectedInfo->subjectNameLen[i] + 1;
                                    st = i+1;
                                    break;
                                }
                                sumOfSubName++;
                            }
                            selectedInfo->currentSubNameLen = sumOfSubName;
                            selectedInfo->stPageIdx = st;
                            selectedInfo->nextPageIdx =fin;
                        }
                        //과목이름 전체길이가 리스트길이보다 작으면 바로 인덱스 갱신
                        else {
                            selectedInfo->stPageIdx = 0;
                            selectedInfo->nextPageIdx = selectedInfo->selectCount;
                            selectedInfo->currentSubNameLen = selectedInfo->totalSubNameLen;
                        }
                        updateSelectedList(serviceState, selectedInfo);
                    }
                }
                clsSuggest();
                resetCounters(istr, cursorPos);
                updateCursor(cursorPos);
			}
            //keboard backspace
            if (keyboardEvent == KEY_BACK) {

                //검색창에 내용이 없거나, 커서의 위치가 검색창 초기위치일 때 변경사항이 없다.
                if (istr->ichCount <= 0 || cursorPos->cursorPosition == 0) {
                    continue;
                }
                showCursor(0);
                clsSuggest();
                // currentCursor가 끝이 아닐 때
                if (istr->ichCount > cursorPos->cursorPosition) {
                    char checkHangle = istr->ichs[cursorPos->cursorPosition - 1].ich[1];
                    //counter 갱신
                    istr->ichCount--;
                    cursorPos->cursorPosition--;
                    if (checkHangle == 0) {
                        cursorPos->charSizeCounter--;
                        cursorPos->currentcharSizeCounter--;
                    }
                    else {
                        cursorPos->charSizeCounter -= 2;
                        cursorPos->currentcharSizeCounter -= 2;
                    }
                    // 포인터의 위치가 중간일때 backspace가 발생하면 charsArray 재배열, insert
                    for (int i = cursorPos->cursorPosition; i < istr->ichCount + 1; i++) {
                        memcpy(istr->ichs[i].ich, istr->ichs[i + 1].ich, 4);
                    }
                    // charsArray를 string으로 합쳐서 buffer에 복사한다.
                    char buffer[SEARCH_WIDTH] = {};
                    for (int k = 0; k < istr->ichCount; k++) {
                        strcat(buffer, istr->ichs[k].ich);
                    }
                    // 검색 제안 갱신
                    if (istr->ichCount > 0) {
                        int resultCount;
                        char** suggestions = NULL;
                        suggestions = getAutocompleteSubjectNames(buffer, &resultCount);
                        printSuggest(suggestions, resultCount);
                    }
                    //cursor을 검색창 초기위치로 이동 후 cslBuffer로 덮어씌우고 다시 검색창 초기위치로 cursor을 이동시킨다.
                    clsSearchBox();
                    //buffer의 내용을 출력하고 현재 커서를 출력 내용 다음으로 이동시킨다.
                    printBuffer(buffer);
                    updateCursor(cursorPos);
                }

                // cursorPosition이 맨 끝에 있는 경우
                if (istr->ichCount == cursorPos->cursorPosition) {
                    char checkHangle = istr->ichs[cursorPos->cursorPosition - 1].ich[1];
                    // counter 갱신
                    istr->ichCount--;
                    cursorPos->cursorPosition--;
                    if (checkHangle == 0) {
                        cursorPos->charSizeCounter--;
                        cursorPos->currentcharSizeCounter--;
                    }
                    else {
                        cursorPos->charSizeCounter -= 2;
                        cursorPos->currentcharSizeCounter -= 2;
                    }
                    // cursor에 위치한 글자 삭제
                    for (int j = 0; j < 4; j++) {
                        istr->ichs[cursorPos->cursorPosition].ich[j] = '\0';
                    }
                    // charsArray를 string으로 합쳐서 buffer에 복사한다.
                    char buffer[SEARCH_WIDTH] = {};
                    for (int k = 0; k < istr->ichCount; k++) {
                        strcat(buffer, istr->ichs[k].ich);
                    }
                    if (istr->ichCount > 0) {
                        int resultCount;
                        char** suggestions = NULL;
                        suggestions = getAutocompleteSubjectNames(buffer, &resultCount);
                        printSuggest(suggestions, resultCount);
                    }
                    //cursor을 검색창 초기위치로 이동 후 cslBuffer로 덮어씌우고 다시 검색창 초기위치로 cursor을 이동시킨다.
                    clsSearchBox();
                    //buffer의 내용을 출력하고 현재 커서를 출력 내용 다음으로 이동시킨다.
                    printBuffer(buffer);
                    updateCursor(cursorPos);
                }
            }
            // keyboard 왼쪽 방향키
            if (keyboardEvent == KEY_LEFT) { 
                //검색창에 내용이 없거나 커서의 위치가 검색창 처음위치일 때 변경사항이 없다.
                if (istr->ichCount <= 0 || cursorPos->cursorPosition <= 0) {
                    continue;
                }
                if (ch[0] != 0)
                    continue;

                //이동하는 단어가 한글인 경우 2칸, 그 외 1칸
                char checkHangle = istr->ichs[cursorPos->cursorPosition - 1].ich[1];
                cursorPos->cursorPosition--;
                if (checkHangle == 0) {
                    cursorPos->currentcharSizeCounter--;
                }
                else {
                    cursorPos->currentcharSizeCounter -= 2;
                }
                updateCursor(cursorPos);
            }
            // keyboard 오른쪽 방향키
            if (keyboardEvent == KEY_RIGHT) {
                if (istr->ichCount >= SEARCH_WIDTH || cursorPos->cursorPosition >= istr->ichCount) {  
                    continue;
                }
                if (ch[0] != 0)
                    continue;
                char checkHangle = istr->ichs[cursorPos->cursorPosition].ich[1];
                cursorPos->cursorPosition++;

                if (checkHangle == 0) {
                    cursorPos->currentcharSizeCounter++;
                }
                else {
                    cursorPos->currentcharSizeCounter += 2;
                }
                updateCursor(cursorPos);
            }
            //keybaord tap service 다음 로직 진행
            if (keyboardEvent == KEY_TAP) {
                serviceState->ssEnum = STOPSEARCH;
            }
            //keybaord ESC 서비스 종료
            if (keyboardEvent == KEY_ESCAPE) {
                serviceState->ssEnum = ESC;
            }
		}
        //mouseEvent
        else if (inputRecord[i].EventType == MOUSE_EVENT) {
           
            MouseState mouseState;
            memset(&mouseState, NULL, sizeof(MouseState));
            //mouse event종류, 좌표
            handleMouseEvent(inputRecord[i].Event.MouseEvent, &mouseState);

            //mouse 오른쪽 한번 클릭, 클릭에 해당하는 과목 삭제
            if (mouseState.mEnum == MOUSE_RIGHT_1) {
                int x = mouseState.x;
                int y = mouseState.y;
                //검색하기 버튼 내의 좌표에 해당하는지 검증
                if (selectedStage(x, y) && selectedInfo->selectCount>0) {
                    int peek = x - ST_SELECTED_X + 1;
                    int sumOfsubName = 0;
                    int stIdx = selectedInfo->stPageIdx;
                    int nextIdx = selectedInfo->nextPageIdx;
                    //좌표에 해당하는 과목이름을 찾음
                    for (int i = stIdx; i < nextIdx; i++) {
                        sumOfsubName += selectedInfo->subjectNameLen[i]+1;
                        if (sumOfsubName >= peek) {
                            peek = i;
                            break;
                        }
                    }
                    //과목 이름에 해당하는 과목정보를 subjectList에서 삭제
                    int check = 0;
                    check = deleteSubject(serviceState->subjectList, peek);
                    if (check == 1) {
                        //선택리스트 전체 길이, 출력되어 있는 현재 길이에 선택된 과목 이름 길이 삭제
                        int totalLen = selectedInfo->totalSubNameLen - selectedInfo->subjectNameLen[peek];
                        int currentLen = selectedInfo->currentSubNameLen - selectedInfo->subjectNameLen[peek];
                        //subjectNameLen을 저장한 리스트 인덱스 정렬
                        for (int i = peek; i < selectedInfo->selectCount; i++) {
                            selectedInfo->subjectNameLen[i] = selectedInfo->subjectNameLen[i + 1];
                        }
                        // 선택 과목 이름 전체 길이가 리스트 길이보다 긴 경우
                        if (totalLen > SELECTED_WIDTH) {
                            if (nextIdx < selectedInfo->selectCount) {
                                selectedInfo->selectCount--;
                                //현재 출력되는 리스트의 마지막인덱스의 다음 인덱스를 더한다.
                                int rearSubName = selectedInfo->subjectNameLen[selectedInfo->nextPageIdx]+1;
                                //더한 길이가 리스트길이보다 길면 삭제만 하고 출력
                                if (SELECTED_WIDTH < currentLen + rearSubName) {
                                    selectedInfo->nextPageIdx--;
                                    selectedInfo->currentSubNameLen = currentLen;
                                }
                                //더한 길이가 리스트길이보다 작으면 추가해서 출력
                                else {
                                    selectedInfo->currentSubNameLen = currentLen + rearSubName;
                                }
                            }
                        }
                        //그 외에는 바로 추가해서 갱신
                        else {
                            selectedInfo->selectCount--;
                            selectedInfo->stPageIdx = 0;
                            selectedInfo->nextPageIdx = selectedInfo->selectCount;
                            selectedInfo->currentSubNameLen = currentLen + selectedInfo->subjectNameLen[selectedInfo->nextPageIdx];

                        }
                        updateSelectedList(serviceState, selectedInfo);
                        updateCursor(cursorPos);
                    }
                }
            }
            //keyboard 왼쪽 클릭
            if (mouseState.mEnum == MOUSE_LEFT_1) {
                int x = mouseState.x;
                int y = mouseState.y;
                //선택 리스트 클릭
                if (selectedStage(x, y)) {
                    int peek = x - ST_SELECTED_X + 1;
                    int sumOfsubName = 0;
                    //선택한 과목 이름을 찾는다.
                    for (int i = selectedInfo->stPageIdx; i < selectedInfo->nextPageIdx; i++) {
                        sumOfsubName += selectedInfo->subjectNameLen[i];
                        if (sumOfsubName > peek) {
                            peek = i;
                            break;
                        }
                        sumOfsubName++;
                    }
                    //선택한 과목 색깔변경
                    updateAndCheckSelectedList(serviceState, selectedInfo ,peek);
                    updateCursor(cursorPos);
                }
                //검색 버튼 클릭
                if (searchButtonStage(x, y)) {
                    // keyboard enter 이벤트 발생시킨다.
                    WORD vk = VK_RIGHT;
                    sendKey(vk);
                    vk = VK_RETURN;
                    sendKey(vk);
                }
            }
            // mouse wheel 위로
            if (mouseState.mEnum == MOUSE_WHEEL_UP) {

                int x = mouseState.x;
                int y = mouseState.y;
                if (selectedStage(x, y)) {
                    if (selectedInfo->nextPageIdx < selectedInfo->selectCount) {
                        //현재 출력된 선택과목 이름에 다음 인덱스에 해당하는 과목이름 길이를 추가
                        int next = selectedInfo->nextPageIdx;
                        int prev = selectedInfo->stPageIdx;
                        int changeLen = selectedInfo->currentSubNameLen + selectedInfo->subjectNameLen[next];
                        //추가된 길이가 리스트 길이보다 작으면 바로 갱신
                        if (changeLen < SELECTED_WIDTH) {
                            selectedInfo->currentSubNameLen = changeLen;
                            selectedInfo->nextPageIdx++;
                        }
                        //리스트 길이보다 길면 앞부분부터 길이를 빼서 리스트길이보다 작게 만들어 갱신
                        else {
                            int peek = prev;
                            for (int i = prev; i <=next; i++) {
                                changeLen -= selectedInfo->subjectNameLen[i];
                                if (changeLen < SELECTED_WIDTH) {
                                    peek = i+1;
                                    break;
                                }
                            }
                            selectedInfo->currentSubNameLen = changeLen;
                            selectedInfo->stPageIdx = peek;
                            selectedInfo->nextPageIdx++;

                        }
                    }
                    updateSelectedList(serviceState, selectedInfo);
                    updateCursor(cursorPos);
                }
            }
            //mouse wheel down, wheel up의 반대로 로직 진행
            if (mouseState.mEnum == MOUSE_WHEEL_DOWN) {
                int x = mouseState.x;
                int y = mouseState.y;
                if (selectedStage(x, y)) {
                    if (selectedInfo->stPageIdx > 0) {
                        selectedInfo->stPageIdx--;
                        int next = selectedInfo->nextPageIdx;
                        int prev = selectedInfo->stPageIdx;
                        int changeLen = selectedInfo->currentSubNameLen + selectedInfo->subjectNameLen[prev];
                        if (changeLen < SELECTED_WIDTH) {
                            selectedInfo->stPageIdx = prev;
                            selectedInfo->currentSubNameLen = changeLen;
                        }
                        else {
                            int peek = next;
                            for (int i = next-1; i > prev; i--) {
                                changeLen -= selectedInfo->subjectNameLen[i];
                                    if (changeLen <= SELECTED_WIDTH) {
                                        peek = i;
                                        break;
                                    }
                            }
                            selectedInfo->stPageIdx = prev;
                            selectedInfo->nextPageIdx = peek;
                            selectedInfo->currentSubNameLen = changeLen;

                        }                        
                        updateSelectedList(serviceState, selectedInfo);
                        updateCursor(cursorPos);
                    }
                }
            }
        }
	}
}
//해당하는 idx의 글자색 변경
void updateAndCheckSelectedList(ServiceState* serviceState, SelectedInfo* selectedInfo, int changeIdx) {
    char listBuffer[ST_SELECTED_X + SELECTED_WIDTH] = {};
    for (int i = selectedInfo->stPageIdx; i < selectedInfo->nextPageIdx; i++) {
        if (i == changeIdx) {
            strcat(listBuffer, "\033[35m"); // 보라색 설정
            strcat(listBuffer, serviceState->subjectList->subject[i]->subjectName);
            strcat(listBuffer, "\033[0m");  //색상 초기화
        }
        else strcat(listBuffer, serviceState->subjectList->subject[i]->subjectName);
        if (i < serviceState->subjectList->subjectSize - 1) {
            strcat(listBuffer, " ");
        }
    }
    showCursor(0);
    clsSelectedBox();
    printSelectedBuffer(listBuffer);

}
//선택 리스트 출력 갱신
void updateSelectedList(ServiceState* serviceState, SelectedInfo* selectedInfo) {
    char listBuffer[ST_SELECTED_X+SELECTED_WIDTH] = {};
    for (int i = selectedInfo->stPageIdx; i < selectedInfo->nextPageIdx; i++) {
        strcat(listBuffer, serviceState->subjectList->subject[i]->subjectName);
        if (i < serviceState->subjectList->subjectSize - 1) {
            strcat(listBuffer, " ");
        }
    }
    showCursor(0);
    clsSelectedBox();
    printSelectedBuffer(listBuffer);
}
//cursorInfo, istr 초기화
void resetCounters(SearchStr* istr, CursorPos* cursorPos) {
    memset(istr->ichs, 0, SEARCH_WIDTH);
    istr->ichCount = 0;
    cursorPos->charSizeCounter = 0;
    cursorPos->cursorPosition = 0;
    cursorPos->currentcharSizeCounter = 0;
}
//검색 글자 버퍼 출력
void printBuffer(char buffer[]) {
    setCursor(ST_SEARCH_X, ST_SEARCH_Y);
    printf("%s", buffer);
}
//선택리스트 버퍼 출력
void printSelectedBuffer(char selected[]) {
    setCursor(ST_SELECTED_X, ST_SELECTED_Y);
    printf("%s", selected);
    setCursor(ST_SEARCH_X, ST_SEARCH_Y);
}
//cursor 위치를 cursroPos에 맞춰 검색창에 갱신
void updateCursor(CursorPos* cursorPos) {
    setCursor(ST_SEARCH_X + cursorPos->currentcharSizeCounter, ST_SEARCH_Y);
    showCursor(1);
}
//검색창 clear
void clsSearchBox(void) {
    char clsBuffer[SEARCH_WIDTH];
    memset(clsBuffer, ' ', SEARCH_WIDTH);
    clsBuffer[SEARCH_WIDTH - 1] = '\0';

    setCursor(ST_SEARCH_X, ST_SEARCH_Y);
    printf("%s", clsBuffer);
}
//선택리스트 clear
void clsSelectedBox(void) {
    char clsBuffer[SELECTED_WIDTH+1];
    memset(clsBuffer, ' ', SELECTED_WIDTH+1);
    clsBuffer[SELECTED_WIDTH] = '\0';

    setCursor(ST_SELECTED_X, ST_SELECTED_Y);
    printf("%s", clsBuffer);
    setCursor(ST_SEARCH_X, ST_SEARCH_Y);
}
//연관 검색어 출력
void printSuggest(char** suggest, int resultCount) {
    showCursor(0);
    for (int i = 0; i < resultCount; i++) {
        setCursor(ST_SUGGEST_X, ST_SUGGEST_Y + i);
        printf("-> %s", suggest[i]);
    }
    free(suggest);
}
//연관 검색창 cls
void clsSuggest(void) {
    showCursor(0);
    char clsBuffer[SELECTED_WIDTH];
    memset(clsBuffer, ' ', SELECTED_WIDTH);
    clsBuffer[SELECTED_WIDTH - 1] = '\0';
    for (int i = 0; i < MAX_SUGGEST; i++) {
        setCursor(ST_SUGGEST_X, ST_SUGGEST_Y+i);
        printf("%s", clsBuffer);
    }
    setCursor(ST_SEARCH_X, ST_SEARCH_Y);
}