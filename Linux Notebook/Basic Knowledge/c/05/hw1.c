#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRNR	10

int main(void)
{
	//hw1	
	int arr[ARRNR] = {};	
	int i, j;
	int max, min;
	int tmp;
	int max_i, min_i;
	int cnt = 0;

	srand(time(NULL));

	//sizeof(数组名)表示整个数组所占用的存储空间
	for (i = 0; i < sizeof(arr)/sizeof(*arr); i++) {
		arr[i] = rand()%100;	
		if (arr[i] % 2 == 0)
			cnt ++;
		if (i == 0) {
			max = min = arr[i];	
			max_i = min_i = i;
		}
		if (arr[i] > max) {
			max = arr[i];
			max_i = i;
		}
		if (arr[i] < min) {
			min = arr[i];
			min_i = i;
		}
		printf("%-3d", arr[i]);
	}
	printf("\n");

	printf("the max:%d, the min: %d, cnt = %d\n", max, min, cnt);
	if (max != arr[ARRNR-1]) {
		arr[max_i] = arr[ARRNR-1];
		arr[ARRNR-1] = max;
		//当最小值在数组的最后一个位置的时候
		if (min_i == ARRNR-1) {
			min_i = max_i;
		}
	}

	if (min != arr[0]) {
		arr[min_i] = arr[0];
		arr[0] = min;	
	}

	for (i = 0; i < ARRNR; i++) {
		printf("%-3d", arr[i]);
	}
	printf("\n");

	//冒泡排序
	//比较趟数
	for (i = 0; i < ARRNR-1; i++) {
		//每一趟，比较的次数	
		for (j = 0; j < ARRNR-i-1; j++) {	
			if (arr[j] > arr[j+1]) {	
				tmp = arr[j];	
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}	

	for (i = 0; i < ARRNR; i++) {
		printf("%-3d", arr[i]);
	}
	printf("\n");


	return 0;
}
