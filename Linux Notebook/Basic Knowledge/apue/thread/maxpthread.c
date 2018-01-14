#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

static void *thr_fun(void *p)
{
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t tid;
	long cnt = 0;
    int	err;

	while (1) {
		err = pthread_create(&tid, NULL, thr_fun, NULL);
		if (err) {
			fprintf(stderr, "pthread_create():%s\n", strerror(err));	
			printf("cnt = %ld\n", cnt);
			exit(1);
		}
		cnt ++;
	}

	exit(0);
}
