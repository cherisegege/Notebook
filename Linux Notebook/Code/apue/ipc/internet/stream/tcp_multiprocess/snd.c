#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#include "proto.h"

//./a.out 1 "hello world" ip
int main(int argc, char **argv)
{
	int tcp_socket;
	struct sockaddr_in raddr;
	struct data_st *snddata;

	if (argc < 4) {
		fprintf(stderr, "%s id string ip\n", argv[0]);
		exit(1);
	}

	tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp_socket < 0) {
		perror("socket()");
		exit(1);
	}

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(atoi(RCVPORT));
	inet_aton(argv[3], &raddr.sin_addr);

	while (1) {
		if (connect(tcp_socket, (void *)&raddr, sizeof(raddr)) < 0) {
			if (errno == EINTR)
				continue;
			perror("connect()");
			goto ERROR;
		}
		break;
	}

	snddata = malloc(sizeof(snddata)+strlen(argv[2]));
	//if error
	snddata->id = htons(atoi(argv[1]));
	strcpy(snddata->str, argv[2]);

	if (send(tcp_socket, snddata, sizeof(snddata)+strlen(argv[2]), 0) < 0 ) {
		perror("send()");
		goto ERROR;
	}

	close(tcp_socket);

	exit(0);
ERROR:
	close(tcp_socket);
	exit(1);
}


