#include <stdio.h>

#define LEFT	1000
#define RIGHT	10000

int isprimer(int number);
int main(void)
{
	int i;
	int cnt = 0;
	
	for (i = LEFT; i <= RIGHT; i++) {
		if (isprimer(i)) {
			printf("%d是一个质数\n", i);	
			cnt ++;
		}	
	}

	printf("共%d个质数\n", cnt);

	return 0;
}

//判断一个整型数是否为质数
int isprimer(int number)
{
	int i;	
	int mark = 0;
	
	for (i = 2; i <= number / 2; i++) {
		if (number % i == 0) {
			mark = 1;
			break;
		}
	}
	if (!mark) {
		return 1;
	}

	return 0;
}

