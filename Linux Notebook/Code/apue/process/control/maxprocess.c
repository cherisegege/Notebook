#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int cnt = 0;
	pid_t pid;


	while (1) {
		pid = fork();
		if (pid < 0)
			break;	
		if (pid == 0) {
			sleep(1);
			exit(0);
		}
		cnt ++;
	}

	perror("fork()");
	printf("cnt = %d\n", cnt);

	exit(0);
}


