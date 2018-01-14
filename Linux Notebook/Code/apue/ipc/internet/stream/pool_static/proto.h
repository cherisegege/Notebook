#ifndef _PROTO_H
#define _PROTO_H

#define RCVPORT	"1999"
#define MAXDATA	512

struct data_st {
	int16_t id;
	int8_t str[1];
}__attribute__((packed));


#endif

