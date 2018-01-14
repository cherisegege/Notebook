#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "proto.h"

int main(int argc, char **argv)
{
	int sd;
	struct stu_st sndata;
	struct sockaddr_in raddr;

	if (argc < 4) {
		fprintf(stderr, "%s id name\n", argv[0]);
		exit(1);
	}

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		perror("socket()");
		exit(1);
	}

	//bind();

	sndata.id = atoi(argv[1]);
	strncpy(sndata.name, argv[2], NAMESIZE);

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(atoi(RCVPORT));
	inet_aton(argv[3], &raddr.sin_addr);

	if (sendto(sd, &sndata, sizeof(sndata), 0, (void *)&raddr, sizeof(raddr)) < 0) {
		perror("sendto()");
		close(sd);
		exit(1);
	}

	close(sd);

	exit(0);
}

