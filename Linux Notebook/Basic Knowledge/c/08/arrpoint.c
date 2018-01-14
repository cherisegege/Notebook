#include <stdio.h>

void rand_arr(int (*p)[5], int n);
int main(void)
{
	int arr[4][5];
	int i, j;

	rand_arr(arr, 4);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 5; j++) {
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}

	return 0;
}

/*
 *随机产生一个4*5的二维数组
 * */
void rand_arr(int (*p)[5], int n)
{
	int i, j;

	for (i = 0; i < n; i++) {
		for (j = 0; j < 5; j++) {
			*(*(p+i)+j) = rand() % 100;
			//p[i][j]
		}
	}
}

