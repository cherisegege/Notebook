#ifndef _PROTO_H
#define _PROTO_H

#define RCVPORT		"2016"
#define NAMESIZE	32

struct stu_st {
	int16_t id;
	int8_t name[NAMESIZE];
}__attribute__((packed));

#endif

