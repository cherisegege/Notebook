#if 0
if (condition?) {
	

}

#endif

#include <stdio.h>

int main(void)
{
/*
	int num;	

	printf("input a number\n");
	scanf("%d", &num);

	if (num % 2 == 0) {
		printf("偶数\n");
	} else {
		printf("奇数\n");
	}
	printf("拜拜\n");
*/
	//判断平年润年
	int year;

	printf("input a year\n");
	scanf("%d", &year);

#if 0
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		printf("%d is a leap\n", year);
	} else {
		printf("%d is not a leap\n", year);
	}
#endif
	if (year % 4 == 0) {
		if (year % 100 != 0) {
			printf("%d is a leap\n", year);
		} else if (year % 400 == 0) {
				printf("%d is a leap\n", year);
		} else {
			printf("%d is not a leap\n", year);
		} 
	} else {
		printf("%d is not a leap\n", year);
	} 


	return 0;
}


