#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define FNAME	"/tmp/out"
#define BUFSIZE	1024
#define PRONR	20

#define PATHNAME	"/etc/services"
#define PROJ_ID		'm'

static int semid;

//fetch semphore
static void P()
{
	struct sembuf sem;

	sem.sem_num = 0;
	sem.sem_op = -1;
	sem.sem_flg = 0;

	semop(semid, &sem, 1);
	//if error

}

//return semphore
static void V()
{
	struct sembuf sem;

	sem.sem_num = 0;
	sem.sem_op = 1;
	sem.sem_flg = 0;

	semop(semid, &sem, 1);
}

static void write_file(void)
{
	FILE *fp;
	int fd;
	char buf[BUFSIZE] = {};
	
	fp = fopen(FNAME, "r+");
	if (NULL == fp) {
		perror("fopen()");
		exit(1);
	}

	fd = fileno(fp);
	//if error

	P();
	fgets(buf, BUFSIZE, fp);
	rewind(fp);
	fprintf(fp, "%d\n", atoi(buf)+1);
	printf("[%d] %d\n",getpid(), atoi(buf)+1 );
	fflush(NULL);
	//sleep(1);
	V();

	fclose(fp);//close可能会引起解锁
}

int main(void)
{
	pid_t pid;
	int i;
	
	//有亲缘关系的进程共用同一个信号量数组
	semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 600);
	//if error

	//one semphore
	semctl(semid, 0, SETVAL, 1);
	//if error

	for (i = 0; i < PRONR; i++) {
		pid = fork();
		if (pid < 0) {
			perror("fork()");
			exit(1);
		}	
		if (pid == 0) {
			write_file();	
			exit(0);
		}
	}

	for (i = 0; i < PRONR; i++)
		wait(NULL);

	semctl(semid, 0, IPC_RMID);

	exit(0);
}

