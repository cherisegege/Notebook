#include <stdio.h>

#define LEFT	100
#define RIGHT	200

int issquare(int num);
int main(void)
{
	int i;

	for (i = LEFT; i <= RIGHT; i++) {
		if (issquare(i)) {	
			printf("%d是一个完全平方数\n", i);
		}
	}

	return 0;
}
/*
 *判断一个整型数是否为完全平方数
 * */
int issquare(int num)
{
	int i;
	int ret = 0;

	if (num < 1) {
		ret = -1;
	}
	if (num == 1)
		ret = 1;

	for (i = 2; i <= num / 2; i++) {
		if (i * i == num) {
			ret = 1;
			break;
		}	
	}

	return ret;
}





