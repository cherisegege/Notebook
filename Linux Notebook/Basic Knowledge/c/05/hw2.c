#include <stdio.h>

#define ARRNR	10

int main(void)
{
	int arr[ARRNR] = {};
	int i;
	int sum = 0;
	int cnt = 0;

	for (i = 0; i < ARRNR; i++) {
		printf("input the %d number\n", i+1);	
		scanf("%d", arr+i);//&arr[i]
		if (arr[i] > 0) {
			sum += arr[i];
			cnt ++;
		}
	}

	printf("cnt = %d, sum = %d\n", cnt, sum);

	return 0;
}
