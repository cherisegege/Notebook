/*
 *功能：验证类型字节数
 * */
#include <stdio.h>

int main(void)
{
	//整型
	printf("char所占的字节个数为%d\n", sizeof(char));
	printf("short所占的字节个数为%d\n", sizeof(short));
	printf("int所占的字节个数为%d\n", sizeof(int));
	printf("long所占的字节个数为%d\n", sizeof(long));
	printf("long long所占的字节个数为%d\n", sizeof(long long));

	//实型
	printf("float所占的字节个数为%d\n", sizeof(float));
	printf("double所占的字节个数为%d\n", sizeof(double));
	printf("long double所占的字节个数为%d\n", sizeof(long double));


	return 0;
}
