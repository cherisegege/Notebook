#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>

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
	int epfd;
	struct epoll_event ev, revt;

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
	
	//创建epoll实例
	epfd = epoll_create(20);
	if (epfd < 0) {
		perror("epoll_create()");
		exit(1);
	}

	//向实例中添加文件描述符
	ev.events = 0;
	ev.data.fd = fd1;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev);	

	ev.events = 0;
	ev.data.fd = fd2;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev);	

	while (fd12.state != STATE_T || fd21.state != STATE_T) {
		//布置现场
		ev.events = 0;
		if (fd12.state == STATE_R) {
			ev.events |= EPOLLIN;
		} 
		if (fd21.state == STATE_W) {
			ev.events |= EPOLLOUT;
		}
		ev.data.fd = fd1;
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd1, &ev);

		ev.events = 0;
		if (fd12.state == STATE_W) {
			ev.events |= EPOLLOUT;
		} 
		if (fd21.state == STATE_R) {
			ev.events |= EPOLLIN;
		}
		ev.data.fd = fd2;
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd2, &ev);

		if (fd12.state < STATE_DV || fd21.state < STATE_DV) {
			while (epoll_wait(epfd, &revt, 1, -1) < 0) {
				if (errno == EINTR)
					continue;
				perror("epoll_wait()");
				exit(1);
			}
		}

		if (((revt.data.fd == fd1) && (revt.events & EPOLLIN)) || \
			((revt.data.fd == fd2) && (revt.events & EPOLLOUT)) || fd12.state > STATE_DV) {
			fsm_drive(&fd12);		
		}
		if (((revt.data.fd == fd2) && (revt.events & EPOLLIN)) || \
			((revt.data.fd == fd1) && (revt.events & EPOLLOUT)) || fd21.state > STATE_DV) {
			fsm_drive(&fd21);		
		}
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

