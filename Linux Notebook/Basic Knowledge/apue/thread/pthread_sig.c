#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

static void sig_handler1(int s)
{
	printf("the signal %d was caught\n", s);
}

static void *thr1(void *p)
{
	while (1) {
		printf("hello\n");
		sleep(1);
	}
}

static void *thr2(void *p)
{
	signal(SIGINT, SIG_DFL);
	while (1) {
		printf("world\n");
		sleep(1);
	}
}

int main(void)
{
	pthread_t tid1, tid2;
	int err, ch;

	signal(SIGINT, sig_handler1);	
	
	err = pthread_create(&tid1, NULL, thr1, NULL);	
	//if error
	err = pthread_create(&tid2, NULL, thr2, NULL);	
	//if error

	while (1) {
		ch = getchar();	
		if (ch == EOF)
			break;
		if (ch == 'a')
			pthread_kill(tid1, SIGINT);
		if (ch == 'b')
			pthread_kill(tid2, SIGINT);
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	exit(0);
}

