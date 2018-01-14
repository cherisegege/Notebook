#ifndef _PROTO_H
#define _PROTO_H

#define MAXMSG		512
#define MAXDATA		(512-sizeof(struct data_st)+1)
#define RCVPORT		"9988"
#define CLIENT1PORT	"1234"
#define CLIENT2PORT	"5678"
#define SERADDR		"172.16.70.210"

struct data_st {
	struct sockaddr_in real_raddr;
	char data[1];	
}__attribute__((packed));


#endif

