#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

/*
 *在多线程模型下，专门设定一个线程处理信号，其他不受干扰
 * */
static sigset_t set;
static int flag = 0;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void *thr_fun(void *p)
{
	int signo;

	while (1) {
		sigwait(&set, &signo);	
		switch(signo) {
			case SIGINT:
				printf("not that!! continue....\n");
				break;
			case SIGQUIT:
				pthread_mutex_lock(&mut);
				flag = 1;
				pthread_cond_signal(&cond);
				pthread_mutex_unlock(&mut);
				pthread_exit(NULL);
				break;
			default:
				printf("i don't know\n");
				break;
		}
	}
}

int main(void)
{
	sigset_t oldset;
	int err;
	pthread_t tid;
	
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);

	pthread_sigmask(SIG_BLOCK, &set, &oldset);

	err = pthread_create(&tid, NULL, thr_fun, NULL);
	//if error

	pthread_mutex_lock(&mut);
	while (flag != 1)
		pthread_cond_wait(&cond, &mut);
	printf("i am back!!!!\n");
	pthread_mutex_unlock(&mut);		

	pthread_join(tid, NULL);

	pthread_sigmask(SIG_SETMASK, &oldset, NULL);

	exit(0);
}


