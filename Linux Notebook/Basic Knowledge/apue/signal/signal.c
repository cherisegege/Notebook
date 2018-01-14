#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

static void sig_handler(int s)
{
	printf("caught %d signal\n", s);
	printf("hahhahahahah!!!\n");
}

int main(void)
{
	void (*sig_ret)(int);
	int i = 0;

	sig_ret = signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);	
	signal(SIGUSR2, sig_handler);

	if (sig_ret == SIG_ERR)
		exit(1);

	while (1) {
		printf("good afternoon!!!\n");
		sleep(1);
		i++;
		if (i == 5)
			signal(SIGINT, sig_ret);
	}

	exit(0);
}

