#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <poll.h>

#define TTY1	"/dev/tty11"
#define TTY2	"/dev/tty12"

#define BUFSIZE	1024

//抽象状态机状态
enum {
	STATE_R = 1,
	STATE_W,
	STATE_DV,
	STATE_EX,
	STATE_T
};

//状态的类型
struct fsm_st {
	int rd;
	int wd;
	int state;
	char buf[BUFSIZE];//读取/写入缓存取
	int len;//成功读取到的字节个数
	int pos;//缓存区中写到的位置
	char *errstr;
};

static void fsm_drive(struct fsm_st *fsm)
{
	int ret;
	//现态
	switch (fsm->state) {
		case STATE_R:
			fsm->len = read(fsm->rd, fsm->buf, BUFSIZE);
			if (fsm->len == 0) {
				//eof
				fsm->state = STATE_T;
			} else if (fsm->len < 0) {
				if (errno == EAGAIN)
					fsm->state = STATE_R;	
				else {
					fsm->errstr = "read()";
					fsm->state = STATE_EX;
				}
			} else {
				fsm->pos = 0;
				fsm->state = STATE_W;
			}
			break;

		case STATE_W:
			ret = write(fsm->wd, fsm->buf+fsm->pos, fsm->len);
			if (ret < 0) {
				if (errno == EAGAIN)
					fsm->state = STATE_W;
				else {
					fsm->errstr = "write()";
					fsm->state = STATE_EX;
				}
			} else{
				fsm->len -= ret;
				fsm->pos += ret;
				if (fsm->len > 0) 
					fsm->state = STATE_W;
				else
					fsm->state = STATE_R;
			}
			break;

		case STATE_EX:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;

		case STATE_T:
			//do sth
			break;
		default:
			abort();
			break;
	}

}

static int maxfds(int m, int n)
{
	return m > n ? m : n;
}

static int relay(int fd1, int fd2)
{
	struct fsm_st fd12;//r1--->w2
	struct fsm_st fd21;//r2--->w1
	int fd1_save, fd2_save;
	struct pollfd pfd[2];

	//非阻塞
	fd1_save = fcntl(fd1, F_GETFL);
	fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);
	fd2_save = fcntl(fd2, F_GETFL);
	fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);
	
	fd12.rd = fd1;
	fd12.wd = fd2;
	fd12.len = 0;
	fd12.state = STATE_R;
	memset(fd12.buf, '\0', BUFSIZE);

	fd21.rd = fd2;
	fd21.wd = fd1;
	fd21.len = 0;
	fd21.state = STATE_R;
	memset(fd21.buf, '\0', BUFSIZE);

	pfd[0].fd = fd1;
	pfd[0].events = 0;
	pfd[0].revents = 0;

	pfd[1].fd = fd2;
	pfd[1].events = 0;
	pfd[1].revents = 0;

	while (fd12.state != STATE_T || fd21.state != STATE_T) {
		//布置现场
		pfd[0].events = 0;
		pfd[0].revents = 0;
		pfd[1].events = 0;
		pfd[1].revents = 0;

		if(fd12.state == STATE_R)
			pfd[0].events |= POLLIN;
		else if(fd12.state == STATE_W)
			pfd[1].events |= POLLOUT;
		if(fd21.state == STATE_R)	
			pfd[1].events |= POLLIN;
		else if(fd21.state == STATE_W)
			pfd[0].events |= POLLOUT;

		if (fd12.state < STATE_DV || fd21.state < STATE_DV) {
			while (poll(pfd, 2, -1) < 0) {
				if (errno == EINTR)
					continue;
				perror("poll()");
				exit(1);
			}
		}

		if (pfd[0].events & POLLIN || pfd[1].events & POLLOUT ||  fd12.state > STATE_DV)
			fsm_drive(&fd12);		
		if (pfd[1].events & POLLIN || pfd[0].events & POLLOUT ||  fd21.state > STATE_DV)
			fsm_drive(&fd21);		
	}	

	fcntl(fd1, F_SETFL, fd1_save);
	fcntl(fd2, F_SETFL, fd2_save);
}

int main(void)
{
	int fd1, fd2;

	fd1 = open(TTY1, O_RDWR);
	if (fd1 < 0) {
		perror("open()");
		exit(1);
	}

	write(fd1, "[beautiful girl]", 16);

	fd2 = open(TTY2, O_RDWR);
	if (fd2 < 0) {
		perror("open");
		close(fd1);
		exit(1);
	}
	write(fd2, "[handsome boy]", 14);

	relay(fd1, fd2);

	exit(0);
}

