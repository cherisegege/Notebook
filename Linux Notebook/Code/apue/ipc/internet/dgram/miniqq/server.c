#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>

#include "proto.h"

int main(void)
{
	int sd;
	int len;
	struct sockaddr_in laddr, raddr;
	struct data_st *rcvdata;
	socklen_t raddrlen;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		perror("socket()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(RCVPORT));
	inet_pton(AF_INET, SERADDR, &laddr.sin_addr);
	if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
		perror("bind()");	
		goto ERROR1;
	}

	rcvdata = malloc(MAXMSG);
	if (rcvdata == NULL) {
		perror("malloc()");
		goto ERROR1;
	}

	raddrlen = sizeof(raddr);
	while (1) {
		len = recvfrom(sd, rcvdata, MAXMSG, 0, (void *)&raddr, &raddrlen);	
		if (len < 0) {
			if (errno == EINTR)
				continue;
			perror("recvfrom()");
			goto ERROR1;
		}
	
		printf("from ip:%s, port:%d\n", inet_ntoa(raddr.sin_addr), ntohs(raddr.sin_port));
		printf("to ip:%s, port:%d\n", inet_ntoa(rcvdata->real_raddr.sin_addr), ntohs(rcvdata->real_raddr.sin_port));

		if (sendto(sd, rcvdata, len, 0, (void *)&rcvdata->real_raddr, sizeof(rcvdata->real_raddr)) < 0) {
			perror("sendto()");
			goto ERROR1;
		}
	}

	close(sd);

	exit(0);
ERROR1:
	close(sd);
	exit(1);
}

