#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define THRNR	5

//互斥量
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
//条件变量
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static pthread_t tids[THRNR] = {};
//临界区
static int pool = 0;

static void *thr_funa(void *p);
static void *thr_funb(void *p);
static void *thr_func(void *p);
static void *thr_fund(void *p);
static void *thr_fune(void *p);

static void sighandler(int s)
{
	pool = -1;
}

int main(void)
{
	int i, err;

	alarm(6);
	signal(SIGALRM, sighandler);

	err = pthread_create(tids+0, NULL, thr_funa, NULL);
	if (err) {
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		exit(1);
	}

	err = pthread_create(tids+1, NULL, thr_funb, NULL);
	if (err) {
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		exit(1);
	}

	err = pthread_create(tids+2, NULL, thr_func, NULL);
	if (err) {
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		exit(1);
	}

	err = pthread_create(tids+3, NULL, thr_fund, NULL);
	if (err) {
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		exit(1);
	}

	err = pthread_create(tids+4, NULL, thr_fune, NULL);
	if (err) {
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		exit(1);
	}

	while(1){
		for (i = 1; i <= 5; i++) {
			pthread_mutex_lock(&mut);
			while (pool != 0 && pool != -1) {
				pthread_cond_wait(&cond, &mut);
			}
			if(pool == -1){
				pthread_mutex_unlock(&mut);
				goto RET;	
			}	
			pool = i;
			pthread_cond_broadcast(&cond);	
			pthread_mutex_unlock(&mut);	
		}	
	}

RET:
	for(i = 0; i < 5; i++){
		pthread_join(tids[i], NULL);
	}
	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cond);
	printf("\n");

	exit(0);
}

static void *thr_funa(void *p)
{
	while (1) {
		pthread_mutex_lock(&mut);
		while (pool != 1 && pool != -1) {
			pthread_cond_wait(&cond, &mut);
			pthread_cond_broadcast(&cond);
		}
		if (pool == -1) {
			pthread_mutex_unlock(&mut);
			break;
		}
		pool = 0;
		pthread_cond_broadcast(&cond);
		write(1, "a ",2);
		pthread_mutex_unlock(&mut);
	}
	
	pthread_exit(NULL);
}

static void *thr_funb(void *p)
{
	int n; 
	int i, mark;

	while (1) {
		pthread_mutex_lock(&mut);
		while (pool != 2 && pool != -1) {
			pthread_cond_wait(&cond, &mut);
			pthread_cond_broadcast(&cond);
		}
		if (pool == -1) {
			pthread_mutex_unlock(&mut);
			break;
		}
		pool = 0;
		pthread_cond_broadcast(&cond);
		write(1, "b ",2);
		pthread_mutex_unlock(&mut);
	}
	
	pthread_exit(NULL);
}

static void *thr_func(void *p)
{
	int n; 
	int i, mark;

	while (1) {
		pthread_mutex_lock(&mut);
		while (pool != 3 && pool != -1) {
			pthread_cond_wait(&cond, &mut);
			pthread_cond_broadcast(&cond);
		}
		if (pool == -1) {
			pthread_mutex_unlock(&mut);
			break;
		}
		pool = 0;
		pthread_cond_broadcast(&cond);
		write(1, "c ",2);
		pthread_mutex_unlock(&mut);
	}
	
	pthread_exit(NULL);
}

static void *thr_fund(void *p)
{
	int n; 
	int i, mark;

	while (1) {
		pthread_mutex_lock(&mut);
		while (pool != 4 && pool != -1) {
			pthread_cond_wait(&cond, &mut);
			pthread_cond_broadcast(&cond);
		}
		if (pool == -1) {
			pthread_mutex_unlock(&mut);
			break;
		}
		pool = 0;
		pthread_cond_broadcast(&cond);
		write(1, "d ",2);
		pthread_mutex_unlock(&mut);
	}
	
	pthread_exit(NULL);
}

static void *thr_fune(void *p)
{
	int n; 
	int i, mark;

	while (1) {
		pthread_mutex_lock(&mut);
		while (pool != 5 && pool != -1) {
			pthread_cond_wait(&cond, &mut);
			pthread_cond_broadcast(&cond);
		}
		if (pool == -1) {
			pthread_mutex_unlock(&mut);
			break;
		}
		pool = 0;
		pthread_cond_broadcast(&cond);
		write(1, "e ",2);
		pthread_mutex_unlock(&mut);
	}
	
	pthread_exit(NULL);
}


