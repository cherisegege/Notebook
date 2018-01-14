/*已知：1990.1.1是星期一
 *问：任意输入年份(90以后的)和月份,打印相应的日历
 * */
#include <stdio.h>

#define BASEYEAR	1990

int main(void)
{
	int year, month;
	int sumdays = 0;
	int firstday, monthdays = 0;
	int i;

	printf("请输入年份和月份(yyy/mmm)\n");
	scanf("%d/%d", &year, &month);

	//求得1990~year-1
	for (i = BASEYEAR; i < year; i++) {
		if ((i%4==0 && i%100!=0) || (i%400==0)) {
			sumdays += 366;	
		} else {
			sumdays += 365;
		}
		/*
		 sumdays += ((i%4==0 && i%100!=0) || (i%400==0) ? 366 : 365);
		 */
	}

	//求得year.1.1~year.month.1
	for (i = 1; i < month; i++) {
		switch (i) {
			case 1: 
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				sumdays += 31;		
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				sumdays += 30;
				break;
			case 2:
				sumdays += (28+((year%4==0 && \
						year%100!=0) || year%400==0));	
				break;
			default:
				break;
		}		
	}
	//year.month.1是星期几
	sumdays += 1;
	firstday = sumdays % 7;
	//printf("firstday = %d\n", firstday);

	//month月有多少天
	switch (month) {
		case 1: case 3:case 5:case 7:case 8:case 10:case 12:
			monthdays = 31;
			break;	
		case 4:case 6:case 9:case 11:
			monthdays = 30;	
			break;
		case 2:
			monthdays = (28+((year%4==0 && \
					year%100!=0) || year%400==0));	
			break;
	}
	//打印日历
	printf("        %d月%d\n", month, year);		
	printf("sun mon tue wed thr fri sat\n");
	for (i = 0; i < firstday; i++) {
		printf("    ");
	}
	for (i = 1; i <= monthdays; i++) {
		printf("%3d%c", i, (firstday+i)%7==0 ? '\n':' ');
	}
	printf("\n");

	return 0;
}






