#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#include "proto.h"

#define DATANUM	5
#define ENDNUM	2

//./a.out data 1 name type
//./a.out end type
int main(int argc, char **argv)
{
	key_t key;
	int msgid;
	msg_t sndmsg;

	if (argc < 2) {
		fprintf(stderr, "Usage ...\n");
		exit(EXIT_FAILURE);
	}
	if (!strcmp(argv[1], "data")) {
		if (argc < DATANUM) {
			fprintf(stderr, "%s data id name type\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	} else if (!strcmp(argv[1], "end")) {
		if (argc < ENDNUM) {
			fprintf(stderr, "%s end \n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	key = ftok(PATHNAME, PROJ_ID);
	//if error

	msgid = msgget(key, 0);
	//if error

	memset(&sndmsg, '0', sizeof(sndmsg));
	if (argc == ENDNUM) {		
		sndmsg.end.mtype = MSGEND;
	} else if (argc == DATANUM) {
		sndmsg.data.mtype = atoi(argv[4]);
		sndmsg.data.id = atoi(argv[2]);
		strncpy(sndmsg.data.name, argv[3], NAMESIZE);
	}
	msgsnd(msgid, &sndmsg, sizeof(sndmsg)-sizeof(long), 0);
	//if error

//	msgctl(msgid, IPC_RMID, 0);

	exit(EXIT_SUCCESS);
}

