#ifndef _PROTO_H
#define _PROTO_H

#define PATHNAME	"/etc/passwd"
#define PROJ_ID		'a'
#define NAMESIZE	30
#define MSGEND		1
#define MSGDATA		10

struct msg_data_st {
	long mtype;/*MUST BE GREAT THAN 1*/
	int id;
	char name[NAMESIZE];	
};

struct msg_end_st {
	long mtype;/*MUST BE 1*/	
};

typedef union {
	long mtype;
	struct msg_data_st data;
	struct msg_end_st end;
}msg_t;

#endif

