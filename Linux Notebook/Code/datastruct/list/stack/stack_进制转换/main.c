#include <stdio.h>

#include "stack.h"

int tentoany(stack_t *s, int src, int des);
int anytoten(int num1, int flag);


int main(void)
{
	int num = 10, num1 = 1010;
	int result;

	stack_t *stack;
	stack = stack_init(sizeof(int));	
	
	result = tentoany(stack, num, 3);
	printf("result = %d\n", result);

	result = anytoten(num1, 3);
	printf("result = %d\n", result);


	return 0;
}

int tentoany(stack_t *s, int src, int des)
{
	int yushu, result = 0;
	int temp;

	while(src){
		yushu = src % des;
		stack_push(s, &yushu);
		src = src / des;
	}
	while(stack_empty(s)){
		stack_pop(s, &temp);
		result = result * 10 + temp;
	}

	return result;
}

int cofunction(int num1, int num2)
{
	int result = 1;

	while(num2){
		result = result * num1;
		num2--;
	}

	return result;
}

int anytoten(int src, int flag)
{
	int result = 0;
	int temp;
	int i = 0;

	while(src){
		temp = cofunction(flag, i);
		result = result + (src % 10) * temp;
		src /= 10;
		i++;
	}

	return result;
}









