/*已知：1990.1.1是星期一
 *问：任意输入年份(90以后的)和月份,打印相应的日历
 * */
#include <stdio.h>

#define BASEYEAR	1990

int isleap(int year);
int day_of_moth(int month, int year);
int main(void)
{
	int year, month;
	int sumdays = 0;
	int firstday, monthdays = 0;
	int i;
	int flag = 0;

	printf("请输入年份\n");
	do {
		scanf("%d", &year);
	} while (!(year >= 1990) && printf("请重新输入\n"));

	do {
		if (!flag) {
			printf("请输入月份\n");
			flag = 1;
		} else {
			printf("请输入1～12之间的月份\n");
		}
		scanf("%d", &month);
	}while (!(month >= 1 && month <= 12));

	//求得1990~year-1
	for (i = BASEYEAR; i < year; i++) {
		sumdays += (365+isleap(i));
	}

	//求得year.1.1~year.month.1
	for (i = 1; i < month; i++) {
		sumdays += day_of_moth(i, year);
	}
	//year.month.1是星期几
	sumdays += 1;
	firstday = sumdays % 7;

	//month月有多少天
	
	monthdays = day_of_moth(month, year);

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

/*
 函数功能：判断年份是否为闰年
 返回值：
 		1	是
		0	不是
 year:
 	待判断的年份
 */
int isleap(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; 
}

/*
	函数的功能： 
	 	判断月份有多少天
	返回值：
   		输入月份的天数
	参数：
		month
		year	
*/
int day_of_moth(int month, int year)
{
	int ret;	

	switch (month) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			ret = 31;	
			break;
		case 4: case 6: case 9: case 11:	
			ret = 30;
			break;
		case 2:
			ret = 28 + isleap(year);
			break;
		default:
			break;
	}

	return ret;
}





