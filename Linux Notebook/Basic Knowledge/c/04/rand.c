/*
 *如何随机产生一个整型数
 	rand(3); srand(3); time(2);
 * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	int num;
	int i;

	srand(time(NULL));
	//将time(NULL)返回的时间戳作为srand的参数

	for (i = 0; i < 10; i++) {
		num = rand() % 100;
		printf("%-3d", num);
	}
	printf("\n");

	return 0;
}

