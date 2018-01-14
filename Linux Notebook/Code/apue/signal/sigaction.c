#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static struct sigaction oldact;

static void sighandler(int s)
{
	int i = 0;

	printf("[%d] %s was caught\n", s, __FUNCTION__);	

	while (i < 5) {
		printf("#");
		fflush(NULL);
		sleep(1);
		i++;
	}
}

static void alarm_handler(int s)
{
	sigaction(SIGINT, &oldact, NULL);
	exit(0);
}

int main(void)
{
	struct sigaction act;

	act.sa_handler = sighandler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);

	sigaction(SIGINT, &act, &oldact);

	alarm(20);

	while (1) {
		printf("!");
		fflush(NULL);
		sleep(1);
	}	

	exit(0);
}

