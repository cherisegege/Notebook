/*
 *	typedef	定义类型的关键字
 * */
#include <stdio.h>

typedef int a;

typedef int *p_t;

typedef void (*pri_t)(const char *);

#define DPS	char *

typedef char *TPS;

int main(void)
{

#if 0
	a b = 10;
	p_t p = &b;

	printf("b = %d, *p = %d\n", b, *p);
#endif
	char ch;
	DPS d1, d2;
	TPS	t1, t2;

	d1 = &ch;
	//d2 = &ch;
	t1 = &ch;
	t2 = &ch;

	return 0;
}

