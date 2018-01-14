#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int mysystem(const char *ptr)
{
	pid_t pid;
	
	pid = fork();
	//if error
	
	if (pid == 0) {
		execl("/bin/sh", "sh", "-c", ptr, NULL);	
		perror("execl()");
		exit(1);
	}

	wait(NULL);

	return 0;
}

int main(void)
{

	printf("%s is starting....\n", __FUNCTION__);

	mysystem("ls -l");

	printf("%s is ending....\n", __FUNCTION__);

	exit(0);
}
