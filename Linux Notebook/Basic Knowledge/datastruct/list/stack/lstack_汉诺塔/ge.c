#include <stdio.h>

#include "lstack.h"


int cnt = 0;

lstack_t *zhuzi[3];

static int init_zhuzi(int n)
{
	int i, j;
	for (i = 0; i < 3; i++) {
		zhuzi[i] = lstack_init(sizeof(int));
		if (i == 0) {
			for (j = n; j > 0; j--) {
				lstack_push(zhuzi[i], &j);
			}
		}
	}
}

static void move(int src, int des)
{
	int temp;

	cnt++;

	lstack_pop(zhuzi[src], &temp);
	lstack_push(zhuzi[des], &temp);
}

static void hano(int n, int src, int des, int temp)
{
	if (n < 1) {
		return;
	}
	if (n == 1) {
		move(src, des);
		return;
	}

	hano(n-1, src, temp, des);
	move(src, des);
	hano(n-1, temp, des, src);

}




int main(void)
{
	int temp;

	init_zhuzi(10);

	hano(10, 0, 1, 2);

	while (!lstack_empty(zhuzi[1])) {
		lstack_pop(zhuzi[1], &temp);
		printf("%d\n", temp);
	}

	printf("一共运算了%d次\n", cnt);


	return 0;
}









