#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "sem.h"

#define MAXSEM  5
#define START	30000000
#define END		30000200
#define NUMBER	(END - START + 1)

static mysem_t *sem = NULL;
static void *pthread_fun(void *p);

int main(void)
{
	int fetch = 0;
	pthread_t tid;
	int pthread_err;
	int i;
	int tmp[NUMBER] = {};

	sem = sem_init(MAXSEM);

	for(i = START; i <= END; i++){
		tmp[i - START] = i;
		fetch = sem_fetch(sem, 1);		
		if(pthread_err = pthread_create(&tid, NULL, pthread_fun, tmp + i - START)){
			fprintf(stderr, "pthread_create failed():%s\n", strerror(pthread_err));
			exit(1);
		}
	}

	fetch = sem_fetch(sem, MAXSEM);
	if(fetch == MAXSEM){
		sem_destroy(sem);
		exit(0);
	}
}

static void *pthread_fun(void *p)
{
	int num = *(int *)p;
	int i, mark = 0;

	pthread_detach(pthread_self());
	for(i = 2; i <= num/2; i++){
		if(num % i == 0){
			mark = 1;
			break;
		}
	}
	if(!mark){
		printf("%d is a primer!\n", num);
	}

	sem_return(sem, 1);
	pthread_exit(NULL);
}

