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

#define PROCNUM	5

static struct sockaddr_in raddr;
static void job_process(int newsd);
int main(void)
{
	int tcp_socket, newsd;
	pid_t pid;
	struct sockaddr_in laddr;
	socklen_t raddrlen;
	int i;

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

	//不能满足有多少连接就能创建多少进程   动态进程池
	raddrlen = sizeof(raddr);
	for (i = 0; i < PROCNUM; i++) {
		pid = fork();
		if (pid < 0) {
			perror("fork()");
			goto ERROR;
		}
		if (pid == 0) {
			while (1) {
				newsd = accept(tcp_socket, (void *)&raddr, &raddrlen);
				if (newsd < 0) {
					if (errno == EINTR)
						continue;
					perror("accept()");
					goto ERROR;
				}
				job_process(newsd);
			}	
			exit(0);
		}
	}
	
	for (i = 0; i < PROCNUM; i++)
		wait(NULL);

	close(tcp_socket);

	exit(0);
ERROR:
	close(tcp_socket);
	exit(1);
}

static void job_process(int newsd)
{
	int len;
	struct data_st *rcvdata;

	rcvdata = malloc(MAXDATA);
	//if error
	memset(rcvdata, '\0', MAXDATA);

	len = recv(newsd, rcvdata, MAXDATA, 0);
	if (len < 0) {
		perror("recv()");
		close(newsd);
		exit(1);
	}
	printf("******FROM IP:%s PORT:%d******\n", inet_ntoa(raddr.sin_addr), ntohs(raddr.sin_port));
	printf("[%d] %s\n", ntohs(rcvdata->id), rcvdata->str);
	close(newsd);
}


