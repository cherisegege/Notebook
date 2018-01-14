#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "proto.h"

int main(void)
{
	int tcp_socket, newsd;
	int len;
	struct sockaddr_in laddr, raddr;
	socklen_t raddrlen;
	struct data_st *rcvdata;

	tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP/*0*/);
	if (tcp_socket < 0) {
		perror("socket()");
		exit(1);
	}

	int val = 1;
	if (setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0 ) {
		perror("setsockopt()");
		goto ERROR;
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(RCVPORT));
	//inet_aton("0.0.0.0", &laddr.sin_addr);
	laddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(tcp_socket, (void *)&laddr, sizeof(laddr)) < 0 ) {
		perror("bind()");	
		goto ERROR;
	}

	if (listen(tcp_socket, 20) < 0) {
		perror("listen()");
		goto ERROR;
	}

	rcvdata = malloc(MAXDATA);
	//if error
	memset(rcvdata, '\0', MAXDATA);

	raddrlen = sizeof(raddr);
	while (1) {
		newsd = accept(tcp_socket, (void *)&raddr, &raddrlen);
		if (newsd < 0) {
			if (errno == EINTR)
				continue;
			perror("accept()");
			goto ERROR;
		}

		len = recv(newsd, rcvdata, MAXDATA, 0);
		if (len < 0) {
			perror("recv()");
			goto ERROR;
		}
		printf("******FROM IP:%s PORT:%d******\n", inet_ntoa(raddr.sin_addr), ntohs(raddr.sin_port));
		printf("[%d] %s\n", ntohs(rcvdata->id), rcvdata->str);
		close(newsd);
	}
	close(tcp_socket);

	exit(0);
ERROR:
	close(tcp_socket);
	exit(1);
}

