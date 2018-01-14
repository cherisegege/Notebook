#include <stdio.h>

#define ARRNR	20

static void arr_rand(int *arr, int n);
static int arr_op(int *arr, int n, int *avg);
int main(void)
{
	int arr[ARRNR] = {};	
	int i;
	int avg, ret;

	arr_rand(arr, ARRNR);

	for (i = 0; i < ARRNR; i++) 
		printf("%d ", arr[i]);
	printf("\n");

	ret = arr_op(arr, ARRNR, &avg);
	printf("more than %d have %d\n", avg, ret);

	return 0;
}

/*
 	随机产生一个20个元素的数组
 */
static void arr_rand(int *arr, int n)
{
	int i;

	srand(time(NULL));

	for (i = 0; i < n; i++) {
		//arr[i] = rand()%100;
		*arr = rand() % 100;
		arr++;
	}
}

/*
	返回值：
   		数组中的大于平均数的个数	
	得出数组的平均值	
 */
static arr_op(int *arr, int n, int *avg)
{
	int i;
	int sum = 0;
	int cnt = 0;

	for (i = 0; i < n; i++) {
		sum += arr[i];	
	}	

	*avg = sum / n;

	for (i = 0; i < n; i++) {
		if (arr[i] > *avg)
			cnt ++;
	}

	return cnt;
}




