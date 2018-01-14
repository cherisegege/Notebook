//分函数实现两个数的交换
#include <stdio.h>

static void swap(int *m, int *n);
int main(void)
{
	int num1, num2;

	printf("input two number\n");
	scanf("%d%d", &num1, &num2);

	printf("before: num1 = %d, num2 = %d\n", num1, num2);

	swap(&num1, &num2);//函数的传参是值传递

	printf("after: num1 = %d, num2 = %d\n", num1, num2);

	return 0;
}

static void swap(int *m, int *n)
{
	int tmp;

	tmp = *m;
	*m = *n;
	*n = tmp;
}

