#include "schedule.h"
#include "subject.h"
#include "cursor.h"
#include "console_input.h"
#include "service.h"
#include "screen.h"

#include <stdio.h>
#include <stdarg.h>

//시작화면
int startService() {
	setConsoleFont();

	int selectService = 0;
	//screen 모듈 호출
	selectService = titleScreen();
	return selectService;
}
// 과목 검색에 필요한 DTO setting
void setSearchingServiceState(SSEnum ssenum, ServiceState* serviceState) {
	serviceState->ssEnum = ssenum;
	serviceState->subjectList = (SubjectList*)calloc(1, sizeof(SubjectList));
	serviceState->scheduleList = NULL;
}
// 시간표 선택에 필요한 DTO setting
void setSelectingServiceState(SSEnum ssenum, ServiceState* serviceState) {
	serviceState->ssEnum = ssenum;
	serviceState->scheduleList = (ScheduleList*)calloc(1, sizeof(ScheduleList));
	serviceState->selectedSchedule = (Schedule*)calloc(1, sizeof(Schedule));
	serviceState->scheduleList->scheduleListSize = 0;
	serviceState->scheduleList->scheduleSize = 0;
}
// txt파일로 저장되어있는 선택했던 시간표로드에 필요한 DTO setting
void setScheduleInfoServiceState(SSEnum ssenum, ServiceState* serviceState) {
	serviceState->ssEnum = ssenum;
	serviceState->selectedSchedule = (Schedule*)calloc(1, sizeof(Schedule));
}
// 동적할당 해제
void freePrevSchduleServiceState(ServiceState* serviceState) {
	for (int i = 0; i < serviceState->selectedSchedule->scheduleSize; i++) {
		free(serviceState->selectedSchedule->scheduleInfo[i]->classInfo);
		free(serviceState->selectedSchedule->scheduleInfo[i]);
	}
	free(serviceState->selectedSchedule);
}
// 동적할당 해제
void freeSearchingServiceState(ServiceState* serviceState) {
	for (int i = 0; i < serviceState->subjectList->subjectSize; i++) {
		for (int j = 0; j < serviceState->subjectList->subject[i]->classSize; j++) {
			free(serviceState->subjectList->subject[i]->classInfo[j]);
		}
	}
}
// 동적할당 해제
void freeSelectingServiceState(ServiceState* serviceState) {
	for (int i = 0; i < serviceState->scheduleList->scheduleListSize; i++) {
		for (int j = 0; j < serviceState->scheduleList->scheduleSize; j++) {
			free(serviceState->scheduleList->schedule[i]->scheduleInfo[j]->classInfo);
			free(serviceState->scheduleList->schedule[i]->scheduleInfo[j]);
		}
		free(serviceState->scheduleList->schedule[i]);
	}
	free(serviceState->scheduleList);
}

// 메인 서비스중 시간표 생성서비스
int createScheduleService() {
	/***********selectSubject logic***********/
	//DTO setting
	ServiceState* serviceState = (ServiceState*)malloc(sizeof(ServiceState));
	setSearchingServiceState(SEARCHING ,serviceState);
	//screen모듈 호출, 검색화면
	searchBox(serviceState);
	//검색화면 중 ESCAPE키 클릭시 해당 서비스 종료, 동적할당 해제
	if (serviceState->ssEnum == ESC) {
		freeSearchingServiceState(serviceState);
		return 0;
	}

	/***********create schedule logic***********/
	//searchBox를 통해 입력받은 과목정보를 DTO에서 받아옴.
	SubjectList* subjects = serviceState->subjectList;
	if (subjects == NULL) {
		return 0;
	}
	//DTO setting
	setSelectingServiceState(CREATING, serviceState);
	//schedule모듈 호출, 과목정보를 전달하여 생성된 schedules를 DTO에 저장
	serviceState->scheduleList = createSchedules(subjects);
	if (serviceState->scheduleList == NULL) {
		return 0;
	}
	freeSearchingServiceState(serviceState);

	/***********select Schedule logic***********/
	scheduleGraph(serviceState);
	if (serviceState->ssEnum == ESC) {
		freeSelectingServiceState(serviceState);
		return 0;
	}
	freeSelectingServiceState(serviceState);
	// save logic
	int selectedAndSaved = 1;
	// schedule모듈 호출, 사용자가 선택한 시간표 schedule.txt로 저장
	int check = saveSelectedSchedule(serviceState->selectedSchedule);
	if (check == 1) {
		selectedAndSaved = 0;
	}
	return selectedAndSaved;
}
// 시간표 수정로직 중 기존에 저장되어있던 시간표에 해당하는 과목들을 DTO에 저장 
void setSubjectList(ServiceState* serviceState) {
	for (int i = 0; i < serviceState->selectedSchedule->scheduleSize; i++) {
		if (serviceState->selectedSchedule->scheduleInfo[i] == NULL) {
			break;
		}
		Subject* searchedSubject = findSubject(serviceState->selectedSchedule->scheduleInfo[i]->subjectName);
		if (searchedSubject != NULL) {
			int check = selectSubject(serviceState->subjectList, searchedSubject);
			if (check == 0) {
				continue;
			}
		}
	}
}
//메인 서비스중 시간표 수정 서비스
int modifyScheduleService() {
	/***********Load Logic for Selected Schedule ***********/
	//Load DTO setting
	ServiceState* serviceState = (ServiceState*)malloc(sizeof(ServiceState));
	setScheduleInfoServiceState(LOADING, serviceState);
	//schedule모듈 호출, schedule.txt로 저장되어있는 시간표정보 load
	int loadcheck = loadSchedule(serviceState->selectedSchedule);
	if (loadcheck != 1) {
		return 0;
	}
	//DTO setting
	setSearchingServiceState(MODIFING, serviceState);
	//load한 시간표정보를 통해 과목정보를 가져와 DTO에 저장
	setSubjectList(serviceState);
	freePrevSchduleServiceState(serviceState);

	/***********selectSubject logic***********/
	searchBox(serviceState);
	if (serviceState->ssEnum == ESC) {
		freeSearchingServiceState(serviceState);
		return 0;
	}
	setSelectingServiceState(CREATING, serviceState);

	/***********create schedule logic***********/
	serviceState->scheduleList = createSchedules(serviceState->subjectList);
	if (serviceState->scheduleList == NULL) {
		return 0;
	}
	freeSearchingServiceState(serviceState);
	//selectSchedule logic
	int selectedAndSaved = 1;
	scheduleGraph(serviceState);
	if (serviceState->ssEnum == ESC) {
		freeSelectingServiceState(serviceState);
		return 0;
	}
	freeSelectingServiceState(serviceState);

	/***********select Schedule logic***********/
	int check = saveSelectedSchedule(serviceState->selectedSchedule);
	if (check == 1) {
		selectedAndSaved = 0;
	}
	return selectedAndSaved;
}
// 메인 로직 중 시간표 확인
int mySchedule() {
	/***********Load Logic for Selected Schedule ***********/
	ServiceState* serviceState = (ServiceState*)malloc(sizeof(ServiceState));
	setScheduleInfoServiceState(LOADING, serviceState);
	int loadcheck = loadSchedule(serviceState->selectedSchedule);
	if (loadcheck != 1) {
		return 0;
	}
	//screen모듈 호출
	printMySchedule(serviceState);
	int escapeNum = myScheduleScreen();
	freePrevSchduleServiceState(serviceState);

	return escapeNum;
}
// 글꼴 크기, 종류 설정
void setConsoleFont() {
	// 고정폭 글꼴로 설정
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 25;  // 글꼴 크기 설정
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");  // 고정폭 글꼴로 설정

	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}