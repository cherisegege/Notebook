#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRNR	20

int main(void)
{
	int arr[ARRNR] = {};
	int i, j;
	int min, m_index;
	int tmp;

	srand(time(NULL));
	for (i = 0; i < ARRNR; i++) {
		arr[i] = rand()%100;
		printf("%d ", arr[i]);
	}
	printf("\n");

	//选择排序
	for (i = 0; i < ARRNR-1; i++) {		
		min = arr[i];	
		m_index = i;
		for (j = i+1; j < ARRNR; j++) {
			if (min > arr[j]) {
				min = arr[j];
				m_index = j;
			}
		}
		if (min != arr[i]) {
			arr[m_index] = arr[i];
			arr[i] = min;
		}
	}

	for (i = 0; i < ARRNR || !printf("\n"); i++) {
		printf("%d ", arr[i]);
	}

	//直接插入排序(从大到小)
	//数组下标为i的元素是待插入的数值
	for (i = 1; i < ARRNR; i++) {
		tmp = arr[i];
		//有序序列
		for (j = i-1; j >= 0; j--) {
			if (arr[j] < tmp) {
				arr[j+1] = arr[j];
			} else {
				break;
			}
		}
		arr[j+1] = tmp;
	}

	for (i = 0; i < ARRNR || !printf("\n"); i++) {
		printf("%d ", arr[i]);
	}

	return 0;
}
