#include "controller.h"
#include "service.h"
#include "console_input.h"
#include <Windows.h>
#include <stdio.h>
void runApp() {
	//serviceNum 0.메인화면 1.시간표생성서비스 2.시간표수정서비스 3.시간표확인서비스 4.서비스종료
	int selectService = 0;
	while (1)
	{
		if (selectService == 0) {
			system("cls");
			// 메인화면 서비스 로직 호출
			selectService = startService();
			if (selectService == 0) {
				printf("errorrrrr");
			}
		}
		if (selectService == 1) {
			// 시간표생성서비스 서비스 로직 호출
			selectService = createScheduleService();
			if (selectService == 0) {
				printf("errorrrrr");
			}
		}
		if (selectService == 2) {
			// 시간표수정서비스 서비스 로직 호출
			selectService = modifyScheduleService();
		}
		if (selectService == 3) {
			// 시간표확인서비스 서비스 로직 호출
			selectService = mySchedule();
		}
		// 프로그램 종료
		if (selectService == 4) {
			break;
		}
	}
}