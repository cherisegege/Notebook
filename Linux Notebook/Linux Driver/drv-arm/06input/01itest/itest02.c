/***************************
 * 鼠标设备事件注入例子1(画方块)
 * 可参考/proc/bus/input/devices
 * 可以在开发板或PC上运行：
 * $>itest02 /dev/input/eventx
 * x轴: value>0则光标向右移动，value<0则向左
 * y轴: value>0则光标向下移动，value<0则向上
 * author: zht
 * date: 2016-04-18
 ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#define MAX		200
#define STEP	2

int main(int argc, char *argv[])
{
	int fd, ret, i;
	struct input_event x, y, sync;

	//检验参数
	if (argc != 2) {
		printf("Usage: $>itest02 /dev/input/eventx\n");
		exit(1);
	}

	//打开设备文件
	fd = open(argv[1], O_RDWR);
	if (fd <= 0) {
        printf("open %s error!\n", argv[1]);
		exit(1);
	}

	//初始化input_event
	x.type = EV_REL;
	x.code = REL_X;
	y.type = EV_REL;
	y.code = REL_Y;
	sync.type = EV_SYN;
	sync.code = 0;
	sync.value = 0;

	//循环读文件，检测信息
	while(1) {
		//向右移动
		for (i=0; i<MAX; i++) {
			x.value = STEP;
			write(fd, &x, sizeof(x));
			write(fd, &sync, sizeof(sync));
			usleep(10000); //10ms
		}
		
		//向下移动
		for (i=0; i<MAX; i++) {
			y.value = STEP;
			write(fd, &y, sizeof(y));
			write(fd, &sync, sizeof(sync));
			usleep(10000);
		}

		//向左移动
		for (i=0; i<MAX; i++) {
			x.value = -STEP;
			write(fd, &x, sizeof(x));
			write(fd, &sync, sizeof(sync));
			usleep(10000);
		}

		//向上移动
		for (i=0; i<MAX; i++) {
			y.value = -STEP;
			write(fd, &y, sizeof(y));
			write(fd, &sync, sizeof(sync));
			usleep(10000);
		}
	}

	close(fd);
	return 0;
}

