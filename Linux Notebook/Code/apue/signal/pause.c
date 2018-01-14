#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sighandler(int s)
{
	printf("hello\n");
}

int main(void)
{
	signal(SIGINT, sighandler);

	pause();

	printf("SIGINT is comming\n");

	exit(0);
}

