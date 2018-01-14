#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include <string.h>

#include "proto.h"

#define BUFSIZE 1024

//./client 127.0.0.1 1123
int main(int argc, char **argv)
{
	int sd;
	struct sockaddr_in laddr,raddr;
	struct data_st *snddata, *rcvdata;
	struct pollfd pfd[2];
	char buf[BUFSIZE] = {};
	
	if (argc < 3) {
		fprintf(stderr, "%s ip port\n", argv[0]);
		exit(0);
	}

	sd = socket(AF_INET, SOCK_DGRAM, 0);	
	if (sd < 0) {
		perror("socket()");
		exit(1);
	}

	//给socket绑定地址
	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(CLIENT1PORT));	
	inet_aton("0.0.0.0", &laddr.sin_addr);
	if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
		perror("bind()");
		close(sd);
		exit(1);
	}

	snddata = malloc(MAXMSG);
	if (snddata == NULL) {
		perror("malloc()");
		close(sd);
		exit(1);
	}
	rcvdata = malloc(MAXMSG);
	//if error
	
	//对话方的地址
	snddata->real_raddr.sin_family = AF_INET;
	snddata->real_raddr.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &snddata->real_raddr.sin_addr);
	//服务器端的地址
	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(atoi(RCVPORT));
	inet_aton(SERADDR, &raddr.sin_addr);

	pfd[0].fd = 0;
	pfd[0].events |= POLLIN;
	pfd[1].fd = sd;
	pfd[1].events |= POLLIN;
	while (1) {
		if (poll(pfd, 2, -1) < 0) {
			if (errno == EINTR)
				continue;
			perror("poll()");
			close(sd);
			exit(1);
		}
		if (pfd[0].revents & POLLIN) {
			fgets(buf, BUFSIZE, stdin);				
			memcpy(snddata->data, buf, strlen(buf)+1);
			if (sendto(sd, snddata, MAXMSG, 0, (void *)&raddr, sizeof(raddr)) < 0) {
				perror("sendto()");
				close(sd);
				exit(1);
			}
		}

		if (pfd[1].revents & POLLIN) {
			if (recvfrom(sd, rcvdata, MAXMSG, 0, NULL, NULL) < 0) {
				perror("recvfrom()");
				close(sd);
				exit(1);
			}
			puts(rcvdata->data);
			fflush(NULL);
		}

	}

	close(sd);

	exit(0);
}


