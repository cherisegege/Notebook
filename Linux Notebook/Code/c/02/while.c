/*
 while (condition) {
	statement; 
 }
 
 for (i = 0; i < 10; i++) {
 
 }
 i = 0;
 while (i < 10) {
 
 	i++;
 }
 * */
#include <stdio.h>

int main(void)
{
	int num;	
	int result, i;

	printf("input a number\n");
	scanf("%d", &num);

	i = 1;
	result = 1;
	//i = result = 1;
	while (i <= num) {
		result *= i;
		i++;	
	}
#if 0
	
	for (i = 1, result = 1; i <= num; i++) {
		result *= i;
	}
#endif

	printf("%d的阶乘是%d\n", num, result);

	return 0;
}


