#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define BUFSIZE	10

static volatile int flag = 0;

static void sighandler(int s)
{
	alarm(1);
	flag = 1;
}

int main(int argc, char **argv)
{
	int fd;
	char buf[BUFSIZE] = {};
	int cnt, pos, ret;

	signal(SIGALRM, sighandler);
	alarm(1);

	if (argc < 2) {
		fprintf(stderr, "Usage...\n");
		goto ERROR;
	}

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

	while (1) {
	//流量控制
	//漏桶  
#if 0
	//查询法
		while (flag == 0)
			;	
#endif
	//通知法
		while (!flag)
			pause();

		flag = 0;

		cnt = read(fd, buf, BUFSIZE);
		if (cnt == 0)
			break;
		if (cnt < 0) {
			if (errno == EINTR)
				continue;
			perror("read()");
			goto READ_ERROR;
		}

		//坚持写够cnt字节
		pos = 0;
		while (1) {
			ret = write(1, buf+pos, cnt);
			if (ret < 0) {
				if (errno == EINTR)
					continue;
				perror("write()");
				goto READ_ERROR;
			}
			cnt -= ret;
			pos += ret;
			if (cnt == 0)
				break;
		}
	}

	close(fd);

	return 0;
ERROR:
	return 1;
READ_ERROR:
	close(fd);
	return 2;
}

