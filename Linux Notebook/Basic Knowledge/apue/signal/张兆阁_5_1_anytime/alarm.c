#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "alarm.h"

#define ALARMSIZE	1024

typedef void (*signalhandler_t)(int);

typedef struct alarmany{
	int sec;
	void *info;
}alarmany_t;

static alarmany_t *adarr[ALARMSIZE] = {};
static signalhandler_t save = NULL;
static int flag = 0, flag_exit = 0;

//信号来了，那了所有alarm的sec都减一,如果为0，则打印相应的信息
static void signalhandler(int s)
{
	int i;

	flag_exit = 0;

	for(i = 0; i < ALARMSIZE; i++){
		if(adarr[i] != NULL){
			adarr[i]->sec -= 1;
			if(adarr[i]->sec <= 0){
				printf("%s\n",adarr[i]->info);
				free(adarr[i]);
				adarr[i] = NULL;
			}
			flag_exit = 1;
		}
	}
	if(!flag_exit)
		exit(EXIT_SUCCESS);
	alarm(1);
}

//找到adarr中可用最小描述符的位置
static int adarrpos(void)
{
	int i;

	for(i = 0; i < ALARMSIZE; i++){
		if(adarr[i] == NULL){
			return i;
		}
	}

	return -ENOSPC;
}

static void alarm_unload(void)
{
	alarm(0);
 	save = signal(SIGALRM, save);
}


static void alarm_load(void)
{
 	save = signal(SIGALRM, signalhandler);
	alarm(1);
	
	atexit(alarm_unload);
}

//注册一个alarmany到adarr中,如果第一次调用， 就加载信号alarm模块
int alarmany_init(int sec, void *info, int size)
{
	int pos;
	alarmany_t *me;

	me = malloc(sizeof(*me));
	if(NULL == me){
		printf("malloc is failed!\n");
	}
	me->info = malloc(size);
	if(me->info == NULL){
		free(me);
		printf("malloc is failed!\n");
	}
	me->sec = sec;
	memcpy(me->info, info, size);

	pos = adarrpos();
	if(pos < 0){
		fprintf(stderr, "%s", strerror(-pos));
	}
	adarr[pos] = me;

	if(flag == 0){
		alarm_load();
		flag = 1;
	}

	return pos;
}




