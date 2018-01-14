/***************************
 * 读取并解析input设备的信息
 * 可参考/proc/bus/input/devices，确定/dev/下input设备文件对应的是哪个具体硬件
 * 可以在开发板或PC上运行：
 * $>itest01 /dev/input/eventx
 * author: zht
 * date: 2016-10-13
 ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>


int main(int argc, char *argv[])
{
	int fd, ret;
	struct input_event t;

	//检验参数
	if (argc != 2) {
		printf("Usage: $>itest01 /dev/input/eventx\n");
		exit(1);
	}

	//打开设备文件
	fd = open(argv[1], O_RDWR);
	if (fd <= 0) {
        printf("open %s error!\n", argv[1]);
		exit(1);
	}

	//循环读文件，检测信息
	while(1) {
		ret = read(fd, &t, sizeof(t));
		if (ret <= 0)
			break;
		switch (t.type) {
		case EV_KEY:
			printf("key %d: %s\n", t.code, \
				(t.value) ? "按下" : "抬起");
			break;
		case EV_REL:
			if (t.code == REL_X)
				printf("rel: x(%d)\n", t.value);
			else if (t.code == REL_Y)
				printf("rel: y(%d)\n", t.value);
			else if (t.code == REL_Z)
				printf("rel: z(%d)\n", t.value);
			else
				printf("rel: %d(%d)\n", t.code, t.value);
			break;
		case EV_ABS:
			if (t.code == ABS_X)
				printf("abs: x(%d)\n", t.value);
			else if (t.code == ABS_Y)
				printf("abs: y(%d)\n", t.value);
			else if (t.code == ABS_Z)
				printf("abs: z(%d)\n", t.value);
			else if (t.code == ABS_MT_TRACKING_ID)
				printf("abs: my_id(%d)\n", t.value);
			else if (t.code == ABS_MT_POSITION_X)
				printf("abs: mt_x(%d)\n", t.value);
			else if (t.code == ABS_MT_POSITION_Y)
				printf("abs: mt_y(%d)\n", t.value);
			else if (t.code == ABS_MT_PRESSURE)
				printf("abs: mt_p(%d)\n", t.value);
			else 
				printf("abs: %d(%d)\n", t.code, t.value);
			break;
		case EV_SYN:
			printf("sync: (%d)(%d)\n", t.code, t.value);
			break;
		}
	}

	close(fd);
	return 0;
}

