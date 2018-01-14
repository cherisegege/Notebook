#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void sighandler(int s)
{
	write(1, "!", 1);
}

int main(void)
{
	int i, j;
	sigset_t set, oldset, tmpset;

	signal(SIGINT, sighandler);

	sigemptyset(&set);
	sigaddset(&set, SIGINT);

	sigprocmask(SIG_UNBLOCK, &set, &oldset);
	for (i = 0; i < 10; i++) {
		sigprocmask(SIG_BLOCK, &set, &tmpset);//阻塞SIGINT
		for (j = 0; j < 5; j++) {
			write(1, "*", 1);
			sleep(1);
		}
		write(1, "\n", 1);
		sigprocmask(SIG_SETMASK, &tmpset, NULL);//解除阻塞SIGINT
	}
	sigprocmask(SIG_SETMASK, &oldset, NULL);

	exit(0);
}

