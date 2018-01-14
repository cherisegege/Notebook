#include <stdio.h>

//宏必须在一行
#define NUM	10

//带参数的宏:注意参数须加括号
#define SQUARE(m) (m)*(m)
/*
 *	typeof(变量名/表达式)	用于获取变量的类型
 * */
#define SWAP(m, n)	\
	do {\
		typeof((m)) tmp;\
		tmp = (m);\
		(m) = (n);\
		(n) = tmp;\
	}while(0)

//两个数的最大值
#define MAX(m, n) (m) > (n) ? (m) : (n)

//#将宏的参数，以字符串的形式呈现
#define SCAN(m)	printf("%s\n", #m)

//##将宏的参数连接
#define TEST(x, y)	SCAN(x##y)

#define	DEBUG 

int main(void)
{
	int n1, n2;
#undef	DEBUG

//条件编译
#ifdef	DEBUG 
	printf("%d\n", SQUARE(5));
	printf("%d\n", SQUARE(5+1));
#endif

	n1 = 10;
	n2 = 20;

	SWAP(n1, n2);

	printf("n1 = %d, n2 = %d\n", n1, n2);
	printf("the max %d and %d is %d\n", n1, n2, MAX(n1, n2));

	SCAN(hello);

	TEST(good, morning);

	return 0;
}

