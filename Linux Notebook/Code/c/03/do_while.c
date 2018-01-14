/*
	do { 
 
 
 
 	}while (condition);
特点：	 
 	至少执行一次, 用于冗错
  */

#include <stdio.h>

int main(void) 
{
	int score;
	int flag = 0;

	do {
		if (flag == 0) {
			printf("请输入一个成绩\n");
			flag = 1;
		} else {
			printf("请重新输入\n");
		}
		scanf("%d", &score);
	} while (!(score >= 0 && score <= 100));

	printf("恭喜你！%d 通过了\n", score);

	return 0;
}




