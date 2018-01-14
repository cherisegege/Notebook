#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *thread_fun(void *p)
{

	printf("%s is called with argument is %d\n", __FUNCTION__, (int)p);

//	return (void *)100;
	pthread_exit((void *)10);
}

int main(void)
{
	int err;
	pthread_t tid;
	int *thret;

	err = pthread_create(&tid, NULL, thread_fun, (void *)1);
	if (err) {
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		exit(1);
	}

	pthread_join(tid, (void **)&thret);
	//thret = (void *)100;
	printf("pthread_join() successfully, status of the thread is %d\n", (int)thret);

	exit(0);
}

