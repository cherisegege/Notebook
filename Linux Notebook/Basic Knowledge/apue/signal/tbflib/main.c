#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "tbf.h"

#define CPS1	1
#define CPS2	5
#define BUFSIZE	1024
#define BURST	100

static int writen(int fd, char *buf, int cnt)
{
	int pos, ret;

	pos = 0;
	while (1) {
		ret = write(fd, buf+pos, cnt);
		if (ret < 0) {
			if (errno == EINTR)
				continue;
			perror("write()");
			return -1;
		}
		cnt -= ret;
		pos += ret;
		if (cnt == 0)
			break;
	}

	return 0;
}

//parent r->argv[1] w->1	child r->argv[2]  w->argv[3]
int main(int argc, char **argv)
{
	int fd, rd, wd;
	char buf[BUFSIZE] = {};
	int cnt, pos, ret;
	int td1, td2;
	pid_t pid;

	if (argc < 4) {
		fprintf(stderr, "Usage...\n");
		goto ERROR;
	}

	pid = fork();
	//if error;
	if (pid > 0) {
		//parent
		while (1) {
			fd = open(argv[1], O_RDONLY);	
			if (fd < 0) {
				if (errno == EINTR)
					continue;
				perror("open()");
				goto ERROR;
			}
			break;
		}
		td1 = tbf_init(CPS1, BURST);
		if (td1 < 0) {
			fprintf(stderr, "tbf_init failed():%s\n", strerror(-td1));	
			exit(1);
		}

		while (1) {
			ret = tbf_fetchtoken(td1, BUFSIZE);
			//if error
			cnt = read(fd, buf, ret);
			if (cnt == 0)
				break;
			if (cnt < 0) {
				if (errno == EINTR)
					continue;
				perror("read()");
				goto READ_ERROR;
			}
			if (ret > cnt)
				tbf_returntoken(td1, ret-cnt);

			//坚持写够cnt字节
			writen(1, buf, cnt);
		}
		tbf_close(td1);
		close(fd);

		wait(NULL);
	} else if (pid == 0) {
	//child	
		rd = open(argv[2], O_RDONLY);	
		//if error
		wd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		//if error

		td2 = tbf_init(CPS2, BURST);
		//if error
		while (1) {
			ret = tbf_fetchtoken(td2, BUFSIZE);
			//if error
			cnt = read(rd, buf, ret);
			if (cnt == 0)
				break;
			if (cnt < 0) {
				if (errno == EINTR)
					continue;
				perror("read()");
				goto READ_ERROR;
			}
			if (ret > cnt)
				tbf_returntoken(td2, ret-cnt);

			//坚持写够cnt字节
			writen(wd, buf, cnt);
		}
		tbf_close(td2);
		exit(0);
	}

	return 0;
ERROR:
	return 1;
READ_ERROR:
	close(fd);
	return 2;
}

