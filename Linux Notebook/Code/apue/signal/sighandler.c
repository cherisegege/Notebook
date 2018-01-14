#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


static void sighandler(int s)
{
	write(1, "!", 1);
	sleep(2);
	write(1, "#", 1);
}

static void sighandler2(int s)
{
	write(1, "?", 1);
}

int main(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler2);

	while (1) {
		write(1, "+", 1);
		sleep(1);
#if 0
		write(1, "-", 1);
		sleep(1);
		write(1, "*", 1);
		sleep(1);
		write(1, "/", 1);
		sleep(1);
#endif
	}

	exit(0);
}

