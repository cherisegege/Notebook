#include <stdio.h>

#define STUNR	100

int main(void)
{
	int score[STUNR] = {};	
	int grade[11] = {};
	int i, j;

	srand(time(NULL));

	for (i = 0; i < STUNR; i++) {
		score[i] = rand()%101;
		printf("%d ", score[i]);
		grade[score[i]/10] ++;
	}
	printf("\n");

	printf("****成绩分布如下****\n");			
	printf("100:");
	for (i = 0; i < grade[10]; i++) {
		printf("* ");
	}
	printf("\n");
	
	for (i = 9; i >= 0; i--) {
		printf("%-2d~%-2d:", i*10, i*10+9);	
		for (j = 0; j < grade[i]; j++) {
			printf("* ");
		}
		printf("\n");
	}

	return 0;
}

