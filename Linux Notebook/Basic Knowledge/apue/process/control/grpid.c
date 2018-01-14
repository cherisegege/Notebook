#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;	

	printf("pid:%ld, pgid:%ld, sid:%ld\n", getpid(), getpgrp(), getsid(0));

	pid = fork();
	//if error

	if (pid == 0) {
		/*child*/
		printf("before set:pid:%ld, pgid:%ld, sid:%ld\n", getpid(), getpgrp(), getsid(0));
		//setpgid(getpid(), 0);

		setsid();

		printf("after set:pid:%ld, pgid:%ld, sid:%ld\n", getpid(), getpgrp(), getsid(0));

		exit(EXIT_SUCCESS);
	}

	wait(NULL);

	exit(0);
}

