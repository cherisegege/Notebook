#include <stdio.h>

int main(void)
{
#if 0
	char str[] = "uplooking";
	const char *p;
	char *const q = str;

	p = (void *)str;

	*p = 'm';	
	*q = 'm';
#endif
	int a = 1, b = 2, c = 3;
	const int *arr1[] = {&a, &b, &c};
	int *const arr2[] = {&a, &b, &c};

	int *p, *q;

	p = &a, q = &b;

	const int **pp= &p;
	int *const *qq = &q;
	int **const mm;

	**pp = 100;

	**qq = 200;
	*qq = &b; 


#if 0
	*arr1[0] = 100;
	*arr1[1] = 200;
#endif
	*arr2[0] = 100;

	arr2[0] = &b;

	return 0;
}

