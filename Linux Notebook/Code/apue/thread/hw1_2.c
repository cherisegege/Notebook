#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRNUM	5

static pthread_mutex_t mut[THRNUM];

static int next_mutex(int n)
{
	return (n+1) % THRNUM;
}

static void *thr_fun(void *p)
{
	int n = (int)p;		

	while (1) {
		pthread_mutex_lock(mut+n);
		printf("%c", 'a'+n);
		fflush(NULL);
		pthread_mutex_unlock(mut+next_mutex(n));	
	}
}

int main(void)
{
	pthread_t tids[THRNUM] = {};	
	int err, i;

	for (i = 0; i < THRNUM; i++) {
		pthread_mutex_init(mut+i, NULL);
		pthread_mutex_lock(mut+i);
		err = pthread_create(tids+i, NULL, thr_fun, (void *)i);
		if (err) {
			fprintf(stderr, "pthread_create(): %s\n", strerror(err));
			exit(1);
		}	
	}

	pthread_mutex_unlock(mut+0);
	alarm(6);

	for (i = 0; i < THRNUM; i++) {
		pthread_join(tids[i], NULL);
		pthread_mutex_destroy(mut+i);
	}

	exit(0);
}

