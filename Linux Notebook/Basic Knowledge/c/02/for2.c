/*
 *100以内所有能被3整除的数
 * */
#include <stdio.h>

//宏  替换
#define NUM	100

int main(void)
{
	int count = 0;	
	int i;

	//遍历1～100之间的所有的整型数
	for (i = 1; i <= NUM; i++) {
		if (i % 3 == 0) {
			printf("%d是3的倍数\n", i);
			count ++;
		}
	}
	printf("共有%d个\n", count);

	return 0;
}
