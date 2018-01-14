/*
 *格式化的输入输出
 * */
#include <stdio.h>

int main(void)
{	
	int age = 18;
	int num1, num2;
	int year, month;

	printf("please input the year and the month(y/m):");
	scanf("%d/%d", &year, &month);

	printf("year = %-10d, month = %010d\n", year, month);

	printf("i am xxx, i'm %d\n", age);	

	scanf("%d%d", &num1, &num2);

	printf("num1 = %d, num2 = %d\n", num1, num2);

	return 0;
}

