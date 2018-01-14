#include <stdio.h>

#define NR	10

int main(void)
{
	int arr[NR][NR] = {};
	int i, j;

	for (i = 0; i < NR; i++) {
		arr[i][0] = 1;		
		printf("%-3d", arr[i][0]);
		for (j = 1; j < NR; j++) {
			if (i >= j && i > 0) {
				arr[i][j] = arr[i-1][j] + arr[i-1][j-1];	
				printf("%-3x", arr[i][j]);
			}
		}
		printf("\n");
	}


	return 0;
}

