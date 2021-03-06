#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define START	30000000
#define END		30000200

#define THRNR	3

//互斥量
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
//条件变量
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//临界区
static int pool = 0;
/*pool == 0	没任务
 *pool > 0  任务
 *pool < 0	任务结束
 * */

static void *thr_fun(void *p)
{
	int n; 
	int i, mark;

	while (1) {
		pthread_mutex_lock(&mut);
		while (pool == 0) {
			pthread_cond_wait(&cond, &mut);
		}
		if (pool == -1) {
			pthread_mutex_unlock(&mut);
			break;
		}
		n = pool;
		pool = 0;
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mut);

		mark = 0;
		for (i = 2; i <= n/2; i++) {
			if (n % i == 0) {
				mark = 1;	
				break;
			}
		}

		if (!mark) {
			printf("%d is a primer\n", n);	
		}
		sleep(1);
	}
	
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t tids[THRNR] = {};
	int i, err;

	for (i = 0; i < THRNR; i++) {
		err = pthread_create(tids+i, NULL, thr_fun, NULL);
		if (err) {
			fprintf(stderr, "pthread_create(): %s\n", strerror(err));
			exit(1);
		}
	}
	
	for (i = START; i <= END; i++) {
		pthread_mutex_lock(&mut);
		while (pool != 0) {
			pthread_cond_wait(&cond, &mut);
		}
		//pool == 0
		pool = i;
		pthread_cond_signal(&cond);	
		pthread_mutex_unlock(&mut);	
	}	

	//任务发放完成
	pthread_mutex_lock(&mut);
	while (pool > 0) {
		pthread_cond_wait(&cond, &mut);
	}
	pool = -1;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mut);
	
	for (i = 0; i < THRNR; i++) {
		pthread_join(tids[i], NULL);
	}	

	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cond);

	exit(0);
}




