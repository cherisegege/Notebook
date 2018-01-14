#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW	4
#define COL	5

/*
	* * * * *
    * * * * *
    * * * * *
    * * * * *
*/

int main(void)
{
	int arr[ROW][COL] = {};	
	int brr[COL][ROW] = {};
	int i, j;
	int max, index;

	srand(time(NULL));

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			arr[i][j] = rand() % 100;	
			printf("%-3d", arr[i][j]);
		}
		printf("\n");
	}

	printf("*******************************\n");
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			if ((i+j) >= 4)
				printf("%-3d", arr[i][j]);
			else
				printf("%-3c", ' ');
		}
		printf("\n");
	}

	//
	for (i = 0; i < ROW; i++) {
		max = arr[i][0];
		for (j = 1; j < COL; j++) {
			if (arr[i][j] > max) {
				max = arr[i][j];
				index = j;			
			}
		}
		if (max != arr[i][0]) {
			arr[i][index] = arr[i][0];
			arr[i][0] = max;
		}
	}

	printf("*********************************\n");
	for (i = 0; i < ROW; i ++) {
		for (j = 0; j < COL; j ++) {
			printf("%-3d", arr[i][j]);
		}	
		printf("\n");	
	}
 
	//转置
	printf("*********************************\n");
	for (i = 0; i < ROW; i ++) {
		for (j = 0; j < COL; j ++) {
			brr[j][i] = arr[i][j];
		}	
	}

	for (i = 0; i < COL; i++) {
		for (j = 0; j < ROW; j++) {
			printf("%-3d", brr[i][j]);
		}
		printf("\n");
	}


	return 0;
}






