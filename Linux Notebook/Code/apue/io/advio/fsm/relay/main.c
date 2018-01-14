#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "relay.h"

#define TTY1	"/dev/tty9"
#define TTY2	"/dev/tty10"
#define TTY3	"/dev/tty11"
#define TTY4	"/dev/tty12"

int main(void)
{
	int fd1, fd2, fd3, fd4;

	fd1 = open(TTY1, O_RDWR);	
	//if error
	write(fd1, "[TTY1] ", 7);

	fd2 = open(TTY2, O_RDWR | O_NONBLOCK);
	//if error
	write(fd2, "[TTY2] ", 7);

	printf("job1:%d\n", relay_addjob(fd1, fd2));

#if 1
	fd3 = open(TTY3, O_RDWR);
	//if error
	write(fd3, "[TTY3] ", 7);

	fd4 = open(TTY4, O_RDWR | O_NONBLOCK);
	//if error
	write(fd4, "[TTY4] ", 7);

	printf("job2:%d\n", relay_addjob(fd3, fd4));
#endif

	pause();

	exit(0);
}
