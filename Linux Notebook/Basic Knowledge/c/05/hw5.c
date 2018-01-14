#include <stdio.h>

int isleap(int year);
int day_of_month(int month, int year);
int main(void)
{
	int year, month, day;
	int sumdays = 0;
	int i;

	printf("请输入有效的年月日(1998/12/5)\n");
	scanf("%d/%d/%d", &year, &month, &day);

	for (i = 1; i < month; i++) {	
		sumdays += day_of_month(i, year);
	}

	sumdays += day;

	printf("%d/%d/%d是这年的第%d天\n", year, month, day, sumdays);

	return 0;
}

int day_of_month(int month, int year)
{
	int ret;

	switch(month) {
		case 1: case 3: case 5: case 7: case 8: \
		case 10: case 12:
			ret = 31;
			break;
		case 4: case 6: case 9: case 11:
			ret = 30;
			break;
		case 2:
			ret = 28+isleap(year);
			break;
		default:
			break;
	}

	return ret;
}

int isleap(int year)
{
	return (year%4==0 && year%100!=0) || year%400==0;
}




