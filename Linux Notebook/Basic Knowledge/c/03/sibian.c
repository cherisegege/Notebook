/*
		****** 
 	   ******
 	  ******
 	 ******
    ******
  */
#include <stdio.h>

#define LINE	10

int main(void)
{
	int i;//控制输出的行数		
	int j;//控制每一行输出的空格的个数
	int k;//控制每一行输出的*号的个数

	for (i = 0; i < LINE; i ++) {
		for (j = 0; j < LINE-i-1; j++) {	
			printf("  ");
		}
		for (k = 0; k < LINE; k++) {
			printf("* ");
		}
		printf("\n");
	}


	return 0;
}



