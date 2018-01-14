#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "sem.h"

struct sem_st{
	int n;
	int max;
	pthread_mutex_t mut;
	pthread_cond_t cond;
};

mysem_t *sem_init(int n)
{
	struct sem_st *me;
	int err;

	if((me = malloc(sizeof(*me))) == NULL){
		fprintf(stderr, "malloc() failed!\n");
		exit(1);
	}
	if(err = pthread_mutex_init(&me->mut, NULL)){
		fprintf(stderr, "pthred_mutex_init failed():%s\n", strerror(err));
	}
	if(err = pthread_cond_init(&me->cond, NULL)){
		fprintf(stderr, "pthread_cond_init failed();%s\n", strerror(err));
	}
	me->n = n;
	me->max = n;

	return me;
}

int sem_fetch(mysem_t *ptr, int num)
{
	struct sem_st *sem = (struct sem_st *)ptr;

	pthread_mutex_lock(&sem->mut);
	while(sem->n < num){
		pthread_cond_wait(&sem->cond, &sem->mut);
	}	
	sem->n -= num;
	pthread_mutex_unlock(&sem->mut);

	return num;
}

int sem_return(mysem_t *ptr, int num)
{
	int tmp;
	struct sem_st *sem = (struct sem_st *)ptr;

	pthread_mutex_lock(&sem->mut);
	tmp = sem->n;
	sem->n += num;
	if(sem->n > sem->max){
		sem->n = sem->max;
	}	
	pthread_cond_broadcast(&sem->cond);
	pthread_mutex_unlock(&sem->mut);

	return sem->n - tmp;
}

void sem_destroy(mysem_t *ptr)
{
	struct sem_st *sem = (struct sem_st *)ptr;

	pthread_mutex_lock(&sem->mut);
	pthread_mutex_unlock(&sem->mut);
	pthread_mutex_destroy(&sem->mut);
	pthread_cond_destroy(&sem->cond);
	free(sem);
}





