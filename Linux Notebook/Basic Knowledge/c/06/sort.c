#include <stdio.h>

static int insert_sort(int *p, int num);
int main(void)
{
	int arr[] = {5,8,9,1,2,4,3,7};
	int i;

	insert_sort(arr, sizeof(arr)/sizeof(arr[0]));

	for (i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
		printf("%d ", arr[i]);
	printf("\n");

	return 0;
}

static int insert_sort(int p[]/*int *p*/, int num)
{
	int i, j;
	int tmp;
	
	for (i = 1; i < num; i++) {
		tmp = p[i];
		for (j = i-1; j >= 0; j--) {
			if (p[j] > tmp)	
				p[j+1] = p[j];
			else
				break;
		}
		p[j+1] = tmp;	
	}

	return 0;
}










