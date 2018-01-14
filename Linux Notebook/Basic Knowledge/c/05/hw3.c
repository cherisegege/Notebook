#include <stdio.h>

#define ARRNR	5

int main(void)
{
	int arr[ARRNR] = {};
	int brr[ARRNR] = {};
	int i;
	
	for (i = 0; i < ARRNR; i ++) {
		printf("input the %d number\n", i+1);
		scanf("%d", arr+i);
	}

	for (i = 0; i < ARRNR; i++) {
		brr[i] = arr[ARRNR-i-1];
		printf("%d ", brr[i]);	
	}
	printf("\n");

	return 0;
}
