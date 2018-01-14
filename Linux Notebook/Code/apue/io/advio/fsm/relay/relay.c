#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "relay.h"

#define BUFSIZE 1024

//状态机的状态
enum {
	STATE_R = 1,
	STATE_W,
	STATE_EX,
	STATE_T
};
//状态机的数据类型
struct fsm_st {
	int rd;
	int wd;
	int state;
	char buf[BUFSIZE];
	int len;
	char *errstr;
	int pos;
};


struct relay_st {
	int fd1, fd2;
	int fd1_save, fd2_save;
	int jobstate;
	struct fsm_st fd12;
	struct fsm_st fd21;
	//...
};

static struct relay_st *jobs[RELAYMAX];
static pthread_mutex_t mut_jobs = PTHREAD_MUTEX_INITIALIZER;  
static pthread_once_t inited = PTHREAD_ONCE_INIT;
static pthread_t tid;

static void fsm_drive(struct fsm_st *fsm)
{
	int ret;

	switch (fsm->state) {
		case STATE_R:		
			fsm->len = read(fsm->rd, fsm->buf, BUFSIZE);
			if (fsm->len == 0) {
				fsm->state = STATE_T;
			} else if (fsm->len > 0) {
				fsm->pos = 0;
				fsm->state = STATE_W;	
			} else {
				if (errno == EAGAIN)
					fsm->state = STATE_R;
				else {
					fsm->errstr = "read()";
					fsm->state = STATE_EX;
				}
			}	
			break;	
		case STATE_W:
			ret = write(fsm->wd, fsm->buf+fsm->pos, fsm->len);	
			if (ret < 0) {
				if (errno == EAGAIN)
					fsm->state = STATE_W;
				else {
					fsm->errstr = "write()";
					fsm->state = STATE_EX;
				}	
			} else {
				fsm->len -= ret;	
				fsm->pos += ret;
				if (fsm->len > 0) {
					fsm->state = STATE_W;
				} else {
					fsm->state = STATE_R;
				}
			}
			break;	
		case STATE_EX:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;
		case STATE_T:
			break;
		default:
			abort();
			break;	
	}

}

static void *thr_fun(void *p)
{
	int i;

	while (1) {
		pthread_mutex_lock(&mut_jobs);
		for (i = 0; i < RELAYMAX; i++) {
			if (jobs[i] !=  NULL && jobs[i]->jobstate != JOB_ENDING) {
				fsm_drive(&jobs[i]->fd12);	
				fsm_drive(&jobs[i]->fd21);	
			}
		}
		pthread_mutex_unlock(&mut_jobs);
	}
}

static void module_unload(void)
{
	int i;

	pthread_cancel(tid);
	pthread_join(tid, NULL);

	pthread_mutex_destroy(&mut_jobs);

	for (i = 0; i < RELAYMAX; i++) {
		if (jobs[i] != NULL) {
			fcntl(jobs[i]->fd1, F_SETFL, jobs[i]->fd1_save);
			fcntl(jobs[i]->fd2, F_SETFL, jobs[i]->fd2_save);
			free(jobs[i]);	
			jobs[i] = NULL;
		}
	}

}

static void sighandler(int s)
{
	exit(1);
}

static void module_load(void)
{
	int err;
	struct sigaction act;

	act.sa_handler = sighandler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	sigaction(SIGINT, &act, NULL);

	err = pthread_create(&tid, NULL, thr_fun, NULL);
	if (err) {
		fprintf(stderr, "pthread_create():%s\n", strerror(err));
		exit(1);
	}
	
	atexit(module_unload);	
}

static int get_free_index_unlock(void)
{
	int i;

	for (i = 0; i < RELAYMAX; i++) {
		if (jobs[i] == NULL)
			return i;
	}

	return -1;
}

int relay_addjob(int fd1, int fd2)
{
	struct relay_st *me;	
	int jobind;	

	pthread_once(&inited, module_load);

	me = malloc(sizeof(*me));
	if (me == NULL)
		return -ENOMEM;
	
	me->fd1_save = fcntl(fd1, F_GETFL);	
	fcntl(fd1, F_SETFL, me->fd1_save | O_NONBLOCK);	

	me->fd2_save = fcntl(fd2, F_GETFL);	
	fcntl(fd2, F_SETFL, me->fd2_save | O_NONBLOCK);	

	me->fd1 = fd1;
	me->fd2 = fd2;
	me->jobstate = JOB_RUNNING;
	me->fd12.rd = fd1;
	me->fd12.wd = fd2;
	me->fd12.state = STATE_R;
	memset(me->fd12.buf, '\0', BUFSIZE);
	me->fd21.rd = fd2;
	me->fd21.wd = fd1;
	me->fd21.state = STATE_R;
	memset(me->fd21.buf, '\0', BUFSIZE);

	pthread_mutex_lock(&mut_jobs);
	jobind = get_free_index_unlock();
	if (jobind < 0) {
		free(me);
		return -ENOSPC;
	}
	jobs[jobind] = me;
	pthread_mutex_unlock(&mut_jobs);

	return jobind;	
}

