#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFSIZE	10

int main(int argc, char **argv)
{
	int fd;
	char buf[BUFSIZE] = {};
	int cnt, pos, ret;

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
	//	usleep(500000);
	}

	close(fd);

	return 0;
ERROR:
	return 1;
READ_ERROR:
	close(fd);
	return 2;
}

