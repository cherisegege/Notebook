#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void cleanup1(void *p)
{
	printf("[%s] the string is %s\n", __FUNCTION__, (char *)p);
}

static void cleanup2(void *p)
{
	printf("[%s] is called\n", __FUNCTION__);
}

static void *thr_fun(void *p)
{

	printf("%s is starting...\n",__FUNCTION__);
	
	pthread_cleanup_push(cleanup1, "this is a test");
	pthread_cleanup_push(cleanup2, NULL);

	printf("%s is ending...\n", __FUNCTION__);

	pthread_exit(NULL);

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
}

int main(void)
{
	pthread_t tid;	
	int err;

	err = pthread_create(&tid, NULL, thr_fun, NULL);
	if (err) {
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		exit(1);
	}

	pthread_join(tid, NULL);

	exit(0);
}

