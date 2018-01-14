/*
	for () { 
 		for () {
 			for () {
				if (xxx) {
					goto ERROR;	
				}
			}
 		}
	}

	
ERROR: 
 	printf("hahha\n");

注意：不建议用goto实现循环
 */
#include <stdio.h>

#define NUM 100

int main(void)
{
	int i;
	int sum = 0;

	i = NUM;
loop:
	sum += i;
	i--;
	if (i > 0) 
		goto loop;

	printf("the result is %d\n", sum);

	return 0;
}


