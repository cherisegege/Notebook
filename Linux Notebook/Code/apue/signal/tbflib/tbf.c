#include <stdlib.h>
#include <signal.h>

#include "tbf.h"

typedef void (*sighandler_t)(int);

typedef struct {
	int burst;
	int cps;
	int token;	
}tbf_t;

static tbf_t *jobs[TBFMAX];
static int inited = 0;
static sighandler_t alarm_save;

static void module_unload(void);
static void sighandler(int s)
{
	int i;

	alarm(1);
	for (i = 0; i < TBFMAX; i++) {
		if (jobs[i] != NULL) {
			jobs[i]->token += jobs[i]->cps;
			if (jobs[i]->token >= jobs[i]->burst)
				jobs[i]->token = jobs[i]->burst;
		}
	}
}

static void module_load(void)
{
	alarm_save = signal(SIGALRM, sighandler);
	alarm(1);

	atexit(module_unload);
}

static void module_unload(void)
{
	alarm(0);
	signal(SIGALRM, alarm_save);

	//tbf_destroyall();	
}

static int tbf_pos(void)
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

	if (!inited) {
		module_load();
		inited = 1;
	}

	me = malloc(sizeof(*me));
	if (me == NULL)
		return -ENOMEM;
	me->burst = burst;	
	me->cps = cps;
	me->token = 0;
	
	pos = tbf_pos();	
	if (pos < 0) {
		free(me);
		return -ENOSPC;
	}

	jobs[pos] = me;

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

	while (jobs[td]->token <= 0)
		pause();
	min = tbf_min(jobs[td]->token, ntoken);
	jobs[td]->token -= min;
	
	return min;
}

int tbf_returntoken(int td, int ntoken)
{
	int save;

	if (td < 0 || td >= TBFMAX || ntoken <= 0)
		return -EINVAL;
	save = jobs[td]->token;

	jobs[td]->token += ntoken;
	if (jobs[td]->token >= jobs[td]->burst)
		jobs[td]->token = jobs[td]->burst;

	return jobs[td]->token - save;
}

int tbf_close(int td)
{
	if (td < 0 || td >= TBFMAX)
		return -EINVAL;
	free(jobs[td]);
	jobs[td] = NULL;

	return 0;
}

void tbf_destroyall(void)
{
	int i;

	for (i = 0; i < TBFMAX; i++) {
		if (jobs[i] != NULL) {
			free(jobs[i]);
			jobs[i] = NULL;
		}
	}
}


