#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>

#include "tbf.h"

typedef void (*sighandler_t)(int);

typedef struct {
	int burst;
	int cps;
	int token;	
	pthread_mutex_t mut;
	pthread_cond_t cond;
}tbf_t;

static pthread_mutex_t mutjobs = PTHREAD_MUTEX_INITIALIZER;
static tbf_t *jobs[TBFMAX];
static pthread_t tid;	
static pthread_once_t inited = PTHREAD_ONCE_INIT;

static void module_unload(void);

static void *thr_func(void *p)
{
	int i;

	while (1) {
		pthread_mutex_lock(&mutjobs);
		for (i = 0; i < TBFMAX; i++) {
			if (jobs[i] != NULL) {
				pthread_mutex_lock(&jobs[i]->mut);
				jobs[i]->token += jobs[i]->cps;
				if (jobs[i]->token >= jobs[i]->burst)
					jobs[i]->token = jobs[i]->burst;
				pthread_mutex_unlock(&jobs[i]->mut);
				pthread_cond_broadcast(&jobs[i]->cond);
			}
		}
		pthread_mutex_unlock(&mutjobs);
		sleep(1);
	}
}

static void module_load(void)
{
	int err;

	err = pthread_create(&tid, NULL, thr_func, NULL);
	if (err) {
		fprintf(stderr, "pthread_create():%s\n", strerror(err));
		exit(1);
	}

	atexit(module_unload);
}

static void module_unload(void)
{
	pthread_cancel(tid);
	pthread_join(tid, NULL);
}

static int tbf_pos_unlock(void)
{
	int i;

	for (i = 0; i < TBFMAX; i++)
		if (jobs[i] == NULL)
			return i;
	return -1;
}

int tbf_init(int cps, int burst)
{
	tbf_t *me;
	int pos;

	pthread_once(&inited, module_load);

	me = malloc(sizeof(*me));
	if (me == NULL)
		return -ENOMEM;
	me->burst = burst;	
	me->cps = cps;
	me->token = 0;

	pthread_mutex_init(&me->mut, NULL);
	pthread_cond_init(&me->cond, NULL);
	
	pthread_mutex_lock(&mutjobs);
	pos = tbf_pos_unlock();	
	if (pos < 0) {
		free(me);
		return -ENOSPC;
	}

	jobs[pos] = me;
	pthread_mutex_unlock(&mutjobs);

	return pos;
}

static int tbf_min(int m, int n)
{
	return m > n ? n : m;
}

int tbf_fetchtoken(int td, int ntoken)
{
	int min;

	if (td < 0 || td >= TBFMAX || ntoken <= 0)
		return -EINVAL;

	pthread_mutex_lock(&jobs[td]->mut);
	while (jobs[td]->token <= 0)
		pthread_cond_wait(&jobs[td]->cond, &jobs[td]->mut);
	min = tbf_min(jobs[td]->token, ntoken);
	jobs[td]->token -= min;
	pthread_mutex_unlock(&jobs[td]->mut);
	
	return min;
}

int tbf_returntoken(int td, int ntoken)
{
	int save;

	if (td < 0 || td >= TBFMAX || ntoken <= 0)
		return -EINVAL;
	save = jobs[td]->token;

	pthread_mutex_lock(&jobs[td]->mut);
	jobs[td]->token += ntoken;
	if (jobs[td]->token >= jobs[td]->burst)
		jobs[td]->token = jobs[td]->burst;
	pthread_mutex_unlock(&jobs[td]->mut);
	pthread_cond_broadcast(&jobs[td]->cond);

	return jobs[td]->token - save;
}

int tbf_close(int td)
{
	if (td < 0 || td >= TBFMAX)
		return -EINVAL;
	pthread_mutex_lock(&mutjobs);
	free(jobs[td]);
	jobs[td] = NULL;
	pthread_mutex_unlock(&mutjobs);

	return 0;
}

void tbf_destroyall(void)
{
	int i;

	pthread_mutex_lock(&mutjobs);
	for (i = 0; i < TBFMAX; i++) {
		if (jobs[i] != NULL) {
			free(jobs[i]);
			jobs[i] = NULL;
		}
	}
	pthread_mutex_unlock(&mutjobs);
}


