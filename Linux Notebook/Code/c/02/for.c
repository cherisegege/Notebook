/**
 *for ( @1; @2; @4) {
 *	@3;
 *}
 *@1:一般用于循环变量的赋初值(只执行一次)
 *@2:循环条件(每次循环都执行)
 *@3:循环体(循环条件满足则执行)
 *@4:循环条件的改变(循环体执行结束后则执行)
 *注意：
 	@1 @2 @3 @4都是可以不写
 */

#include <stdio.h>

int main(void)
{
#if 0
	int number;		
	int i, sum = 0;

	printf("input a number\n");
	scanf("%d", &number);

	for (i = 1; i <= number; ++i) {
		sum = sum + i;	
		//sum += i;
	}	

	printf("%d的前n项和为%d\n", number, sum);
#endif

	//死循环
	for (; ;) {
	
	}

	return 0;
}

