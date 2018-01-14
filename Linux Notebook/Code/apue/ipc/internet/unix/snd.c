#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include "proto.h"

int main(void)
{
	int sd;
	struct sockaddr_un runaddr;
	
	sd = socket(AF_UNIX, SOCK_STREAM, 0);
	//if error

	runaddr.sun_family = AF_UNIX;
	memcpy(runaddr.sun_path, PATHNAME, strlen(PATHNAME)+1);
	connect(sd, (void *)&runaddr, sizeof(runaddr));
	//if error

	write(sd, "hello", 6);

	close(sd);

	exit(0);
}

