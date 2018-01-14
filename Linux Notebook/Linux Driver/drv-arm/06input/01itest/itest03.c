/***************************
 * 鼠标设备事件注入例子2(画圆)
 * 可参考/proc/bus/input/devices
 * 可以在开发板或PC上运行：
 * $>itest03 /dev/input/eventx
 * x轴: value>0则光标向右移动，value<0则向左
 * y轴: value>0则光标向下移动，value<0则向上
 * author: zht
 * date: 2016-04-18
 ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#define RS		100	//半径
#define	STEP	1

int main(int argc, char *argv[])
{
	double step, point;
	double ypos[RS];
	int diff[RS];
	int fd, ret, i, j;
	struct input_event x, y, sync;

	//检验参数
	if (argc != 2) {
		printf("Usage: $>itest03 /dev/input/eventx\n");
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

	//初始化Y坐标变化的数组
	for (i=0; i<RS; i++) {
		ypos[i] = abs(sqrt(RS*RS - (RS-i)*(RS-i)) + 0.5);
		if (i==0)
			diff[i] = 0;
		else 
			diff[i] = ypos[i] - ypos[i-1];
	}

	//循环读文件，检测信息
	while(1) {
		//第四象限
		for (i=0; i<RS; i++) {
			x.value = STEP;
			y.value = -diff[i];
			write(fd, &x, sizeof(x));
			write(fd, &sync, sizeof(sync));
			write(fd, &y, sizeof(y));
			write(fd, &sync, sizeof(sync));
			usleep(10000);
		}
		
		//第一象限
		for (i=0; i<RS; i++) {
			x.value = STEP;
			y.value = diff[RS-i-1];
			write(fd, &x, sizeof(x));
			write(fd, &sync, sizeof(sync));
			write(fd, &y, sizeof(y));
			write(fd, &sync, sizeof(sync));
			usleep(10000);
		}
		
		//第二象限
		for (i=0; i<RS; i++) {
			x.value = -STEP;
			y.value = diff[i];
			write(fd, &x, sizeof(x));
			write(fd, &sync, sizeof(sync));
			write(fd, &y, sizeof(y));
			write(fd, &sync, sizeof(sync));
			usleep(10000);
		}
		
		//第三象限
		for (i=0; i<RS; i++) {
			x.value = -STEP;
			y.value = -diff[RS-i-1];
			write(fd, &x, sizeof(x));
			write(fd, &sync, sizeof(sync));
			write(fd, &y, sizeof(y));
			write(fd, &sync, sizeof(sync));
			usleep(10000);
		}
	}

	close(fd);
	return 0;
}

