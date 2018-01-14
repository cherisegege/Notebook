#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include "sem.h"

struct sem_st{
	int n;
	pthread_mutex_t mut;
	pthread_cond_t cond;
};

mysem_t *sem_init(int n)
{
	struct sem_st *new;

	new = malloc(sizeof(*new));
	if (new == NULL)
		return NULL;
	new->n = n;
	pthread_mutex_init(&new->mut, NULL);
	pthread_cond_init(&new->cond, NULL);

	return new;
}

int sem_fetch(mysem_t *s, int num)
{
	struct sem_st *sem = s;

	if (num < 0) {
		return -EINVAL;
	}

	pthread_mutex_lock(&sem->mut);		
	while (sem->n < num)	
		pthread_cond_wait(&sem->cond, &sem->mut);
	sem->n -= num;
	pthread_mutex_unlock(&sem->mut);

	return 0;
}

int sem_return(mysem_t *s, int num)
{
	struct sem_st *sem = s;

	if (num < 0) 
		return -EINVAL;
	pthread_mutex_lock(&sem->mut);
	sem->n += num;
	pthread_cond_broadcast(&sem->cond);
	pthread_mutex_unlock(&sem->mut);

	return 0;
}

void sem_destroy(mysem_t *s)
{
	struct sem_st *sem = s;

	pthread_mutex_destroy(&sem->mut);
	pthread_cond_destroy(&sem->cond);
	free(sem);
}

