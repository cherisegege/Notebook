/*
 *运算符
 * */
#include <stdio.h>

int main(void)
{
#if 0
	int a, b, c; 

	a = 10;
	b = a++;//++在后，先参与运算在自增
	c = ++b;//++在前，先自增再参与运算

	printf("a = %d, b = %d, c = %d\n", a, b, c);
	
	printf("sizeof(a) = %d\n", sizeof(a));
	printf("sizeof(int) = %d\n", sizeof(int));
	printf("sizeof a = %d\n", sizeof a);
	//printf("sizeof int = %d\n", sizeof int);错误的
#endif
	
#if 0
	//逗号运算符	
	int num1 = 10;
	int num2 = 20;
	int num3;

	num3 = (num1++, num2++, num1+num2, num1);
	printf("num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);

	num3 = num1 > num2 ? num1++:num2--;
	printf("num1 = %d, num2 = %d, num3 = %d\n", num1, num2, num3);
#endif		
#if 0
	//移位运算
	int num = 10;	
	//	1010

	num = num >> 1;
	printf("num = %d\n", num);
	//num 101
	num = num << 2;
	printf("num = %d\n", num);	

	num = 67;
	//1000011
	//如何将任意一个整型数的第三位置成1
	num = num | (1u << 2);
	printf("num = %d\n", num);

	num = 67;
	//如何将任意一个整型数的第二位清成0
	num = num & ~(1u << 1);
	printf("num = %d\n", num);
#endif
	//类型的强转
	int a = 260;
	//1 00000100
	char b;

	b = a;
	
	//a + b

	printf("b = %hhd\n", b);

	int m = -20;
	unsigned int n = 6;
	int p;

	p = (m + n) > 0 ? 1 : 0;	

	printf("p = %d\n", p);

	return 0;
}

