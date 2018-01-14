#include <stdio.h>

int main(void)
{
	int ch;	
	int num;

	printf("请输入一个整型数\n");
	scanf("%d", &num);
	//清除缓存区
	while (getchar() != '\n')
		;
	printf("请输入一个字符\n");
	ch = getchar();

	printf("num = %d, ch = %c\n", num, ch);

	return 0;
}

