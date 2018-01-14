#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE	1024

int main(void)
{
	int shmid;
	pid_t pid;
	char *childptr, *parentptr;

	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | 6000);
	if (shmid < 0) {
		perror("shmget()");
		exit(1);
	}

	pid = fork();
	if (pid < 0) {
		perror("fork()");
		goto ERROR;
	}
	if (pid == 0) {
		childptr = shmat(shmid, NULL, 0);
		if (childptr == (void *)-1) {
			perror("shmat()");
			goto ERROR;
		}
		memcpy(childptr, "hello world", 12);
		shmdt(childptr);
		exit(0);
	}
	wait(NULL);
	parentptr = shmat(shmid, NULL, 0);	
	//if error
	puts(parentptr);
	shmdt(parentptr);

	shmctl(shmid, IPC_RMID, NULL);
	exit(0);
ERROR:
	shmctl(shmid, IPC_RMID, NULL);
	exit(1);

}

