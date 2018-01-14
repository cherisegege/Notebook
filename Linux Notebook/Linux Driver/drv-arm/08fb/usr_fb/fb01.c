/********************
 * fb设备的测试例子1
 * 获得fb文件的信息
 * $>fb01
 * $>fb01 /dev/graphics/fb0
 * //如果没有参数，默认针对/dev/fb0
 * //指定要打开哪个fb，如果是4412，则可以支持多个窗口
 * Author: zht
 * Date: 2016-11-26
 ********************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define DEF_FB	"/dev/fb0"

int main(int argc, char *argv[])
{
	int fd, ret;
	struct fb_fix_screeninfo fix_info;
	struct fb_var_screeninfo var_info;

	//1.打开fb文件
	if (argc == 1) { 
		fd = open(DEF_FB, O_RDWR);
	} else if (argc == 2) {
		fd = open(argv[1], O_RDWR);
	} else {
		printf("Usage: $>fb01 /dev/fbx\n");
		exit(1);
	}
	if (fd < 0) {
		printf("open %s error\n", argv[1]);
		exit(1);
	}
	printf("open %s\n", argv[1]);

	//2.获得并显示fb_fix_screeninfo
	ret = ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
	if (ret) {
		printf("%s: get fix info error %d\n", argv[1], ret);
		close(fd);
		exit(1);
	}

	printf("The fix_screeninfo:\n\
	id = %s\n\
	smem_start = %x\n\
	smem_len = %u\n\
	type = %u\n\
	visual = %u\n\
	xpanstep = %u\n\
	ypanstep = %u\n\
	ywrapstep = %u\n\
	line_length = %u\n",	\
	fix_info.id,		\
	fix_info.smem_start,	\
	fix_info.smem_len, 	\
	fix_info.type, 		\
	fix_info.visual, 	\
	fix_info.xpanstep, 	\
	fix_info.ypanstep, 	\
	fix_info.ywrapstep, 	\
	fix_info.line_length
	);

	//3.获得var信息
	ret = ioctl(fd, FBIOGET_VSCREENINFO, &var_info);
	if (ret) {
		printf("%s: get var info error %d\n", argv[1], ret);
		close(fd);
		exit(1);
	}

	printf("The fb_var_screeninfo:\n\
	xres = %u\n\
	yres = %u\n\
	xres_virtual = %u\n\
	yres_virtual = %u\n\
	xoffset = %u\n\
	yoffset = %u\n\
	bits_per_pixel = %u\n\
	grayscale = %u\n\
	red.offset = %u\n\
	red.length =%u\n\
	green.offset = %u\n\
	green.length = %u\n\
	blue.offset = %u\n\
	blue.length = %u\n\
	transp.offset = %u\n\
	transp.length = %u\n\
	nonstd = %u\n\
	activate = %u\n\
	height = %d\n\
	width = %d\n\
	pixclock = %u\n\
	left_margin = %u\n\
	right_margin = %u\n\
	upper_margin = %u\n\
	lower_margin = %u\n\
	hsync_len = %u\n\
	vsync_len = %u\n\
	sync = %u\n\
	vmode = %u\n\
	rotate = %u\n",\
	var_info.xres,\
	var_info.yres,\
	var_info.xres_virtual,\
	var_info.yres_virtual,\
	var_info.xoffset,\
	var_info.yoffset,\
	var_info.bits_per_pixel,\
	var_info.grayscale,\
	var_info.red.offset,\
	var_info.red.length,\
	var_info.green.offset,\
	var_info.green.length,\
	var_info.blue.offset,\
	var_info.blue.length,\
	var_info.transp.offset,\
	var_info.transp.length,\
	var_info.nonstd,\
	var_info.activate,\
	var_info.height,\
	var_info.width,\
	var_info.pixclock,\
	var_info.left_margin,\
	var_info.right_margin,\
	var_info.upper_margin,\
	var_info.lower_margin,\
	var_info.hsync_len,\
	var_info.vsync_len,\
	var_info.sync,\
	var_info.vmode,\
	var_info.rotate);

	close(fd);
	return 0;
}

