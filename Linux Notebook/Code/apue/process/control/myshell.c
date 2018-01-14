#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <glob.h>

static void parse(const char *ptr, glob_t *res);
int main(void)
{
	char *lineptr = NULL;
	size_t n = 0;
	pid_t pid;
	glob_t globres;
	int i;

	while (1) {
		printf("[uplooking@you]# ");
		if (getline(&lineptr, &n, stdin) < 0) {
			perror("getline()");
			exit(1);
		}
		lineptr[strlen(lineptr)-1] = 0;

		if (!strcmp(lineptr, "exit"))	
			break;
//		parse(lineptr, &globres);

		fflush(NULL);
		pid = fork();
		if (pid < 0) {
			perror("fork()");
			exit(1);
		}
		if (pid == 0) {
//			execvp(globres.gl_pathv[0],globres.gl_pathv);
			execl("/bin/sh", "sh", "-c", lineptr, NULL);
			perror("execvp()");
			exit(1);
		}
		wait(NULL);
	}

	exit(EXIT_SUCCESS);
}

static void parse(const char *p, glob_t *res)
{
	char *ret;
	int flag = 0;
	char *ptr = (char *)p;

	while (1) {
		ret = strsep(&ptr, " \t\n");
		if (ret == NULL)
			break;
		//ls      -l	
		if (*ret == 0)
			continue;
		glob(ret, (flag * GLOB_APPEND) | GLOB_NOCHECK, NULL, res);
		flag = 1;
	}
}





