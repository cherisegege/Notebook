#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "proto.h"

#define MINFREEPROC	5
#define MAXFREEPROC	10
#define MAXPOOLPROC	30

#define SIGNOTIFY	SIGUSR1

enum {
	STATE_FREE = 1,
	STATE_BUSY,
	STATE_INIT
};

struct pool_st {
	pid_t pid;
	int state;
};

static struct pool_st *jobs;
static int free_cnt, busy_cnt;
static int sd;

static int add_proc_pool(void);
static void child_job(int index);
static int del_proc_pool(void);

static void notify_handler(int s)
{

}

int main(void)
{
	int i, ret;
	sigset_t set, oldset, saveset;
	struct sigaction act;
	struct sockaddr_in laddr;

	act.sa_handler = notify_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGNOTIFY, &act, NULL);	

	sd = socket(AF_INET, SOCK_STREAM, 0);
	//if error
	
	int val = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	//if error

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(RCVPORT));
	inet_aton("0.0.0.0", &laddr.sin_addr);
	bind(sd, (void *)&laddr, sizeof(laddr));
	//if error

	listen(sd, 200);

	//进程间共享空间
	jobs = mmap(NULL, MAXPOOLPROC*sizeof(struct pool_st), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (jobs == MAP_FAILED) {
		perror("mmap()");
		exit(1);
	}

	for (i = 0; i < MAXPOOLPROC; i++) {
		jobs[i].pid = -1;
		jobs[i].state = STATE_INIT;
	}

	//push 5 process into pool
	for (i = 0; i < MINFREEPROC; i++) {
		if ((ret = add_proc_pool()) < 0) {
			fprintf(stderr, "add_proc_pool():%s\n", strerror(-ret));
			exit(1);
		}	
	}

	//block sigset
	sigemptyset(&set);
	sigaddset(&set, SIGNOTIFY);
	sigprocmask(SIG_UNBLOCK, &set, &saveset);
	sigprocmask(SIG_BLOCK, &set, &oldset);
	
	while (1) {
		//信号驱动调用进程遍历进程池
		sigsuspend(&oldset);	
		
		//遍历进程池
		free_cnt = 0;
		busy_cnt = 0;
		for (i = 0; i < MAXPOOLPROC; i++) {
			if (jobs[i].pid == -1)
				continue;	
			if (kill(jobs[i].pid, 0)) {
				jobs[i].pid = -1;
				continue;
			}
			if (jobs[i].state == STATE_FREE)
				free_cnt ++;	
			else if (jobs[i].state == STATE_BUSY)
				busy_cnt ++;
		}	

		//more than MAXFREEPROC 
		if (free_cnt > MAXFREEPROC) {
			for (i = 0; i < (free_cnt - MAXFREEPROC); i++) {
				if ((ret =  del_proc_pool()) < 0) {
					fprintf(stderr, "del_proc_pool():%s\n", strerror(-ret));
					exit(1);
				}
			}
		}
		if (free_cnt < MINFREEPROC) {
			for (i = 0; i < (MINFREEPROC-free_cnt); i++) {
				add_proc_pool();	
				//if error
			}	
		}
	
		for (i = 0; i < MAXPOOLPROC; i++) {
			if (jobs[i].pid == -1)
				putchar('-');
			else if (jobs[i].state == STATE_FREE)
				putchar('F');
			else if (jobs[i].state == STATE_BUSY)
				putchar('B');
		}
		printf("\n");
	} 
	sigprocmask(SIG_SETMASK, &saveset, NULL);

	exit(0);
}

static int add_proc_pool(void)
{
	int i;
	pid_t pid;

	if ((free_cnt + busy_cnt) >= MAXPOOLPROC)
		return -ENOSPC;
	for (i = 0; i < MAXPOOLPROC; i++) {
		if (jobs[i].pid == -1)
			break;
	}	
	
	fflush(NULL);
	pid = fork();	
	if (pid < 0) {
		return -errno;
	}
	if (pid == 0) {
		//child
		child_job(i);	
		exit(0);
	}
	jobs[i].pid = pid;
	jobs[i].state = STATE_FREE;

	return 0;
}

static int del_proc_pool(void)
{
	int i;

	if (free_cnt < MINFREEPROC)
		return -EINVAL;

	for (i = 0; i < MAXPOOLPROC; i++) {
		if (jobs[i].pid != -1 && jobs[i].state == STATE_FREE)
			break;
	}
	kill(jobs[i].pid, SIGKILL);	
	jobs[i].pid = -1;
	jobs[i].state= STATE_INIT;

	return 0;
}

static void child_job(int index)
{
	int newsd;
	struct sockaddr_in raddr;
	socklen_t raddrlen;
	struct msg_st rcv;
	
	raddrlen = sizeof(raddr);
	while (1) {	
		jobs[index].state = STATE_FREE;
		kill(getppid(), SIGNOTIFY);
		newsd = accept(sd, (void *)&raddr, &raddrlen);
		if (newsd < 0) {
			perror("accept()");
			exit(1);
		}
		jobs[index].state = STATE_BUSY;
		kill(getppid(), SIGNOTIFY);

		read(newsd, &rcv, sizeof(rcv));
		rcv.ch ++;
		if (rcv.ch > 'z')
			rcv.ch = 'a';

		write(newsd, &rcv, sizeof(rcv));
	
		sleep(5);

		close(newsd);
	}
}









