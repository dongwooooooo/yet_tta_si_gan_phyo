#include "static_data.h"
#include "cursor.h"
#include <stdio.h>
//메인화면
void printTitleScreen(int x, int y) {
	setCursor(x, y);
	printf("  ***     * *             *      *****              *          *                        *    *****\n");
	setCursor(x, y+1);
	printf(" *   * **** *    *******  *      *   *        *     *   *****  *     *******   *******  *    *   *\n");
	setCursor(x, y + 2);
	printf("*     *   * *    *        *      *   *       * *    *       *  ****    * *     *        *    *   *\n");
	setCursor(x, y + 3);
	printf(" *   * **** *    *        ****   *   *      *   *   *       *  *     *******   *        ****  * * \n");
	setCursor(x, y + 4);
	printf("  ***     * *    *******  *       * *      *     *  *     *    *       * *     *******  *      *  \n");
	setCursor(x, y + 5);
	printf("    *     *               *        *                *     *            * *              *     *** \n");
	setCursor(x, y + 6);
	printf("   * *   * *              *       ***               *     ******    *********           *     *** \n");
	setCursor(x, y + 7);
	printf("  *   * *   *                     ***                                                             \n");

	setCursor(22, 25);
	printf("시간표 생성은 1번!          시간표 수정은 2번!          시간표 확인은 3번!          나가기는 4번..ㅜ-ㅜ");
}
//검색창 선택화면
void printSearchButton(int startX, int startY) {
	setCursor(startX - 1, startY);
	printf(" **********");
	setCursor(startX - 1, startY + 1);
	printf(" * Select *");
	setCursor(startX - 1, startY + 2);
	printf(" **********");
}
//검색창
void printSearchBox(int startX, int startY) {
	setCursor(startX - 1, startY - 2);
	printf("수강할 과목을 입력하세요.");
	setCursor(startX-1, startY-1);
	printf("*******************************************************************************");
	setCursor(startX-1, startY);
	printf("*                                                                             *");
	setCursor(startX-1, startY+1);
	printf("*******************************************************************************");
	//검색창 35.155~ 111.15
	//선택 35.35~ 111,35
	//검색버튼 114.14(16) 123.14(16)
}
//선택 과목 리스트
void printSelectedBox(int startX, int startY) {
	setCursor(startX - 1, startY - 2);
	printf("선택한 과목");
	setCursor(startX - 1, startY - 1);
	printf("*******************************************************************************");
	setCursor(startX - 1, startY);
	printf("*                                                                             *");
	setCursor(startX - 1, startY + 1);
	printf("*******************************************************************************");
}
//시간표 선택창 시간표 틀
void printScheduleGraph(int x, int y) {
	setCursor(x, y); printf("o--------------------------------------------------------------o\n");
	setCursor(x, y + 1); printf("|"); setCursor(x + 8, y + 1); printf("|    월    |"); setCursor(x + 8 + 11, y + 1); printf("|    화    |"); setCursor(x + 8 + 11 * 2, y + 1); printf("|    수    |"); setCursor(x + 8 + 11 * 3, y + 1); printf("|    목    |"); setCursor(x + 8 + 11 * 4, y + 1); printf("|    금    |");
	setCursor(x, y + 2); printf("|==============================================================|");
	setCursor(x, y + 3); printf("| 1교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 4); printf("|       |          |          |          |          |          |\n"); 
	setCursor(x, y + 5); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 6); printf("| 2교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 7); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 8); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 9); printf("| 3교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 10); printf("|       |          |          |          |          |          |\n"); 
	setCursor(x, y + 11); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 12); printf("| 4교시 |          |          |          |          |          |\n");
	setCursor(x, y + 13); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 14); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 15); printf("| 5교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 16); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 17); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 18); printf("| 6교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 19); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 20); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 21); printf("| 7교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 22); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 23); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 24); printf("| 8교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 25); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 26); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 27); printf("| 9교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 28); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 29); printf("o--------------------------------------------------------------o");
	setCursor(x, y + 30); printf("**TAP: 정렬방식 변경**");
}
//시간표 확인창 시간표 틀
void printMyScheduleGraph(int x, int y) {
	setCursor(x, y); printf("o--------------------------------------------------------------o\n");
	setCursor(x, y + 1); printf("|"); setCursor(x + 8, y + 1); printf("|    월    |"); setCursor(x + 8 + 11, y + 1); printf("|    화    |"); setCursor(x + 8 + 11 * 2, y + 1); printf("|    수    |"); setCursor(x + 8 + 11 * 3, y + 1); printf("|    목    |"); setCursor(x + 8 + 11 * 4, y + 1); printf("|    금    |");
	setCursor(x, y + 2); printf("|==============================================================|");
	setCursor(x, y + 3); printf("| 1교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 4); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 5); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 6); printf("| 2교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 7); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 8); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 9); printf("| 3교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 10); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 11); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 12); printf("| 4교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 13); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 14); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 15); printf("| 5교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 16); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 17); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 18); printf("| 6교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 19); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 20); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 21); printf("| 7교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 22); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 23); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 24); printf("| 8교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 25); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 26); printf("|--------------------------------------------------------------|");
	setCursor(x, y + 27); printf("| 9교시 |          |          |          |          |          |\n"); 
	setCursor(x, y + 28); printf("|       |          |          |          |          |          |\n");
	setCursor(x, y + 29); printf("o--------------------------------------------------------------o");
}
//시간표 선택창 시간표선택 버튼
void printSelectAcceptButton(int x, int y) {
	setCursor(x - 1, y);
	printf("*******************");
	setCursor(x - 1, y + 1);
	printf("* Select schedule *");
	setCursor(x - 1, y + 2);
	printf("*******************");
}
//시간표 선택창 왼쪽 화살
void printSelectLeftButton(int x, int y) {
	setCursor(x, y-2); printf("    *\n");
	setCursor(x, y-1); printf("  ***\n");
	setCursor(x, y);   printf("**********\n");
	setCursor(x, y+1); printf("  ***\n");
	setCursor(x, y+2); printf("    *\n");


}
//시간표 선택창 왼쪽 화살
void printSelectRightButton(int x, int y) {
	setCursor(x, y-2); printf("     *\n");
	setCursor(x, y-1); printf("     ***\n");
	setCursor(x, y);   printf("**********\n");
	setCursor(x, y+1); printf("     ***\n");
	setCursor(x, y+2); printf("     *\n");


}
