
#include <stdio.h>

/*
	递归：	
		1.找终止条件
		2.找递归点
*/


/*
 *前n项和
 如果函数的实现在调用之前，函数可以不额外声明
 * */
int sumofall(int num)
{
	if (num == 0)
		return 0;
	return num + sumofall(num-1);
}

int febcacii(int n);
long fact(long m);
int main(void)
{
	int n;
	int i;

	printf("input a number\n");
	scanf("%d", &n);

	printf("%d的前n项和为%d\n", n, sumofall(n));
	printf("%d的阶乘为%ld\n", n, fact(n));

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
	if (n == 1 || n == 2)
		return 1;
	return febcacii(n-1)+febcacii(n-2);
}

/*
 *n的阶乘
 * */
long fact(long m)
{
	if (m == 0)	
		return 1;
	return m * fact(m-1);
}






