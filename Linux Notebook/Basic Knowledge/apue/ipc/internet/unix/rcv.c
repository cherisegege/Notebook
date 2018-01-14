#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <signal.h>

#include "proto.h"

static void sig_handler(int s)
{
	unlink(PATHNAME);
	exit(0);
}

int main(void)
{
	int unix_sd, newsd;
	struct sockaddr_un unaddr, runaddr;
	socklen_t runaddrlen;
	struct msg_st *rcv;

	signal(SIGINT, sig_handler);

	unix_sd = socket(AF_UNIX, SOCK_STREAM, 0);
	//if error;
	
	unaddr.sun_family = AF_UNIX;
	memcpy(unaddr.sun_path, PATHNAME, strlen(PATHNAME)+1);

	if (bind(unix_sd, (void *)&unaddr, sizeof(unaddr)) < 0) {
		perror("bind()");
		close(unix_sd);
		exit(1);
	}

	listen(unix_sd, 20);
	//if error

	rcv = malloc(MAXMSG);
	//if error

	runaddrlen = sizeof(runaddr);
	while (1) {
		newsd = accept(unix_sd, (void *)&runaddr, &runaddrlen);
		//if error

		printf("rcv %d bytes\n", read(newsd, rcv, MAXMSG));
		printf("%s\n", rcv);
		close(newsd);
	}

	close(unix_sd);

	exit(0);
}

