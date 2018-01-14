#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRNUM	5

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int job = 0;//标识当前应该抢到锁的线程编号

static void *thr_fun(void *p);
int main(void)
{
	pthread_t tids[THRNUM] = {};	
	int err;
	int i;

	for (i = 0; i < THRNUM; i++) {
		err = pthread_create(tids+i, NULL, thr_fun, (void *)i);
		if (err) {
			fprintf(stderr, "pthread_create():%s\n", strerror(err));
			exit(1);
		}	
	}

	alarm(6);

	for (i = 0; i < THRNUM; i++)
		pthread_join(tids[i], NULL);

	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cond);

	exit(0);
}

static int next_job(int cur)
{
	return (cur+1)%THRNUM;
}

static void *thr_fun(void *p)
{
	int index = (int)p;//线程编号	'a'+index	

	while (1) {
		pthread_mutex_lock(&mut);
		while (job != index)	
			pthread_cond_wait(&cond, &mut);
		printf("%c", 'a'+index);
		fflush(NULL);
		pthread_mutex_unlock(&mut);	
		job = next_job(job);//期望下一次抢到锁的线程编号	
		pthread_cond_broadcast(&cond);
	}

	pthread_exit(NULL);
}




