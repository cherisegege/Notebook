#include <signal.h>
#include <stdio.h>

void sig_handler(int unused)
{
	printf("气死我了!!!!!!!!!\n");	
}

int main(void)
{
	signal(2, sig_handler);

	while (1) {
		printf("求杀死！！！！\n");
		sleep(1);
	}

	return 0;
}

