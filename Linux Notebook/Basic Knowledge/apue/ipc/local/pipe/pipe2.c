#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 1024

static int chile_rpipe_wfile(int fd, const char *file);
static int parent_rfile_wpipe(int fd, const char *file);

int main(int argc, char **argv)
{
	int pfd[2] = {};
	pid_t pid;

	if (argc < 3) {
		fprintf(stderr, "Usage...\n");
		exit(1);
	}	

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
		close(pfd[1]);
		chile_rpipe_wfile(pfd[0], argv[2]);
		close(pfd[0]);
		exit(0);
	}
	
	close(pfd[0]);
	parent_rfile_wpipe(pfd[1], argv[1]);
	close(pfd[1]);

	wait(NULL);

	exit(0);
}

static int parent_rfile_wpipe(int fd, const char *file)
{
	FILE *fp = NULL;
	char buf[BUFSIZE] = {};

	fp = fopen(file, "r");
	if (fp == NULL) {
		perror("fopen()");
		exit(1);
	}

	while (1) {	
		if (fgets(buf, BUFSIZE, fp) == NULL)
			break;
		write(fd, buf, strlen(buf));
	}
	
	fclose(fp);

	return 0;
}

static int chile_rpipe_wfile(int fd, const char *file)
{
	char buf[BUFSIZE] = {};
	int cnt;
	FILE *fp = NULL;

	fp = fopen(file, "w");
	if (fp == NULL)
		exit(1);

	while (1) {	
		cnt = read(fd, buf, BUFSIZE);
		if (cnt < 0) {
			if (errno == EINTR)
				continue;	
			perror("read()");
			exit(1);
		}
		if (cnt == 0)
			break;
		fwrite(buf, 1, cnt, fp);
	}

	fclose(fp);

	return 0;
}


