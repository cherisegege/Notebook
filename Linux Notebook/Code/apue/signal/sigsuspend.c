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

	//signal(SIGINT, sighandler);
	signal(SIGRTMIN+6, sighandler);

	sigemptyset(&set);
	//sigaddset(&set, SIGINT);
	sigaddset(&set, SIGRTMIN+6);

	sigprocmask(SIG_UNBLOCK, &set, &oldset);
	sigprocmask(SIG_BLOCK, &set, &tmpset);

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 5; j++) {
			write(1, "*", 1);
			sleep(1);
		}
		write(1, "\n", 1);
		sigsuspend(&tmpset);
		/*
		 *@1将mask集设置成tmpset
		 *@2pause()
		 *@3恢复之前mask集
		 * */
	}
	sigprocmask(SIG_SETMASK, &oldset, NULL);

	exit(0);
}

