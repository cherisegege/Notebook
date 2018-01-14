#ifndef _RELAY_H
#define _RELAY_H

#define RELAYMAX 1024
//任务队列的状态
enum {
	JOB_RUNNING = 1,
	JOB_CANCEL,
	JOB_ENDING
};

struct job_statattr {
	int fd1;
	int fd2;
	int jobstate;	
	//...
};

/*fd1,fd2:加入状态机的文件描述符
 *返回值:
		>0	 	状态机任务下标
		-EINVAL	参数非法
		-ENOSPC	任务队列已满
		-ENOMEM	空间开辟失败
 * */
int relay_addjob(int fd1, int fd2);

/*
 *id:收尸的任务下标
 *status:获取任务状态
 * */
int relay_waitjob(int id, struct job_statattr *status);


#endif

