#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define LSEEK_FILE	"./lseekfile"
#define BUFSIZE		10

int main(void)
{
	int fd;	
	char buf[BUFSIZE] = {};
	
	fd = open(LSEEK_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd < 0) {
		perror("open()");
		goto ERROR;
	}

	write(fd, "aaaaaaaaaabbbbbbbbbbcccccccccc", 30);

	if (lseek(fd, 10, SEEK_SET) < 0) {
		perror("lseek()");
		close(fd);
		goto ERROR;
	}
	read(fd, buf, BUFSIZE);
	puts(buf);

	memset(buf, '\0', BUFSIZE);

	lseek(fd, 5, SEEK_CUR);
	//if error
	read(fd, buf, BUFSIZE);
	puts(buf);

	close(fd);

	return 0;
ERROR:
	return 1;
}

