/*
 * 任意输入一个2016年的月份，计算出其月份1号距离2016.1.1号有多少天
 * */
#include <stdio.h>

int main(void)
{
	int month;		
	int sumdays = 0;

	printf("input the month of 2016\n");
	scanf("%d", &month);

	if (month < 1 || month > 12) {
		printf("invalued month\n");	
	} else {
		switch (month - 1) {
			case 11:
				sumdays += 30;		
			case 10:
				sumdays += 31;	
			case 9:
				sumdays += 30;
			case 8:
				sumdays += 31;	
			case 7:
				sumdays += 31;	
			case 6:
				sumdays += 30;	
			case 5:
				sumdays += 31;	
			case 4:
				sumdays += 30;	
			case 3:
				sumdays += 31;	
			case 2:
				sumdays += (28+(2016%4 == 0 && 2016%100 != 0 || 2016%400 == 0));	
			case 1:
				sumdays += 31;	
				break;
			default:
				break;
		}	
		//month.1
		sumdays += 1;

		printf("1.1~~%d.1共%d天\n", month, sumdays);
	}

	return 0;
}

