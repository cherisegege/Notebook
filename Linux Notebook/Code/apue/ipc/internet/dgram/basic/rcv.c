#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>	/*for socket*/
#include <netinet/in.h>	
#include <netinet/ip.h> /*for ip*/
#include <arpa/inet.h>

#include "proto.h"

int main(void)
{
	int sd;
	int len;
	struct sockaddr_in laddr, raddr;
	socklen_t raddrlen;
	struct stu_st rcvdata;

	sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP/*0*/);
	if (sd < 0) {
		perror("socket()");
		exit(1);
	}

	//为socket绑定本地地址
	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(RCVPORT));
	inet_aton("0.0.0.0", &laddr.sin_addr);

	if (bind(sd, (struct sockaddr *)&laddr, sizeof(laddr)) < 0) {
		perror("bind()");
		close(sd);
		exit(1);
	}

	/***********/
	raddrlen = sizeof(raddr);
	while (1) {	
		len = recvfrom(sd, &rcvdata, sizeof(rcvdata), 0, (void *)&raddr, &raddrlen);
		if (len < 0) {
			if (errno == EINTR)
				continue;
			perror("rcvfrom()");
			close(sd);
			exit(1);
		}

		printf("the massage from:[%d][%s]\n", ntohs(raddr.sin_port), inet_ntoa(raddr.sin_addr));
		printf("id:%d, name:%s\n", rcvdata.id, rcvdata.name);
	}

	close(sd);

	exit(0);
}

