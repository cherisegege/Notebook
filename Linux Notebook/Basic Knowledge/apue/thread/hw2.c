#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRNUM	20
#define FLNAME	"/tmp/out"
#define BUFSIZE 1024

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void *thr_func(void *p)
{
	FILE *fp = NULL;
	char buf[BUFSIZE] = {};

	fp = fopen(FLNAME, "r+");
	if (fp == NULL)
		pthread_exit((void *)1);

	pthread_mutex_lock(&mut);
	fgets(buf, BUFSIZE, fp);	
	rewind(fp);
	fprintf(fp, "%d\n", atoi(buf)+1);
	fflush(NULL);
	sleep(1);
	pthread_mutex_unlock(&mut);
	fclose(fp);//刷新缓存

}

int main(void)
{
	pthread_t tids[THRNUM] = {};
	int i, err;

	for (i = 0; i < THRNUM; i++) {
		err = pthread_create(tids+i, NULL, thr_func, NULL);
	// if error	
	}

	for (i = 0; i < THRNUM; i++) 
		pthread_join(tids[i], NULL);

	pthread_mutex_destroy(&mut);

	exit(0);
}

