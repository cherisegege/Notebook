#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(void)
{
	int pfd[2] = {};	
	pid_t pid;
	char buf[BUFSIZE] = {};

	if (pipe(pfd) < 0) {
		perror("pipe()");
		exit(1);
	}

	pid = fork();
	if (pid < 0) {
		perror("fork()");	
		exit(1);
	}

	if (pid == 0) {
		//child
		close(pfd[0]);		
		write(pfd[1], "hello world", 11);
		close(pfd[1]);
		exit(0);
	}
	close(pfd[1]);
	read(pfd[0], buf, BUFSIZE);
	puts(buf);
	close(pfd[0]);

	wait(NULL);

	exit(0);
}

