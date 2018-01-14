#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

static void sighandler(int s)
{
	kill(0, SIGQUIT);
}

static void sighandler2(int s)
{
	printf("bye-bye\n");
	exit(1);
}

int main(void)
{
	pid_t pid;
	int i;	
	
	signal(SIGALRM, sighandler);	
	signal(SIGQUIT, sighandler2);	

	for (i = 0; i < 5; i++) {
		pid = fork();
		//if error
		if (pid == 0) {
			while (1) {
				printf("pid: %ld, ppid:%ld, pgid:%ld, sid:%ld\n",\
						getpid(), getppid(), getpgrp(), getsid(0));
				sleep(1);
			}
			exit(0);
		}
	}

	alarm(5);	

	while (1);

	exit(0);
}

