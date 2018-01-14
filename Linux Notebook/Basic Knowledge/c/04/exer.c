
#include <stdio.h>

/*
 *前n项和
 如果函数的实现在调用之前，函数可以不额外声明
 * */
int sumofall(int num)
{
	int sum = 0;	

	for (; num > 0; num--) {
		sum += num;
	}

	return sum;
}

int febcacii(int n);
int main(void)
{
	int n;
	int i;

	printf("input a number\n");
	scanf("%d", &n);

	printf("%d的前n项和为%d\n", n, sumofall(n));

	for (i = 1; i <= 20; i++) {
		printf("%d ", febcacii(i));
	}
	printf("\n");
	
	return 0;
}

/*
 *求非波那契数列的第n项
 * */
int febcacii(int n)
{
	int num1, num2;
	int cur, i;
	if (n == 1 || n == 2) {
		return 1;//如果一个函数是有返回值的，一旦return 值的时候函数即结束
		//如果一个函数没有返回值，则return
	} else if (n < 1) {
		return -1;	
	} else {
		num1 = num2 = 1;	
		for (i = 2; i < n; i++) {
			cur = num1 + num2;			
			num1 = num2;
			num2 = cur;				
		}

		return cur;
	}
}










