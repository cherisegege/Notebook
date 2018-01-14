#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "proto.h"

int main(void)
{
	key_t key;
	int msgid;
	msg_t rcvmsg;
	int len;

	//get key
	key = ftok(PATHNAME, PROJ_ID);
	if (key < 0) {
		perror("ftok()");
		exit(EXIT_FAILURE);
	}

	//get msgq id
	msgid = msgget(key, IPC_CREAT | 600);
	if (msgid < 0) {
		perror("msgget()");
		exit(EXIT_FAILURE);
	}

	//recv msg
	while (1) {
		len = msgrcv(msgid, &rcvmsg, sizeof(rcvmsg)-sizeof(long), 0, 0);
		if (len < 0) {
			if (errno == EINTR)
				continue;
			perror("msgrcv()");
			break;
		}
		if (rcvmsg.mtype == MSGEND) {
			printf("The end\n");
			break;
		}
		printf("recv the data:%d %s\n", rcvmsg.data.id, rcvmsg.data.name);
	}

	msgctl(msgid, IPC_RMID, NULL);

	exit(EXIT_SUCCESS);
}

