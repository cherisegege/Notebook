#include <stdio.h>

#define NUM 100

int main(void)
{
	int num;	

	while (1) {
		printf("input a number\n");
		scanf("%d", &num);
		if (num == 0) {
			printf("once again\n");
			continue;//结束本次循环，继续下一次循环
		}
		if (num < 0)
			break;//直接跳出循环
		printf("%d/%d = %d\n", NUM, num, NUM/num);
	}
#if 0
	for (i = 0; i < 100; i++) {
		if (xxxx)
			continue;
	}

#endif

	return 0;
}
