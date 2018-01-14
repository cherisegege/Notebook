/********************
 * 在fb0上循环显示图片
 * 目前只支持点阵图片，用picture目录下的mkpic工具制作
 * $>fb02 ./picture/allimg.bin
 * 如果有兴趣，可以结合mkpic工具实现直接在fb0上输出jpeg图片
 * Author: zht
 * Date: 2016-11-26
 ********************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define DEF_FB	"/dev/fb0"


/************************
 * 显示1幅图片
 * fb: fb缓冲区的指针
 * pic: 图片缓冲区的基地址
 * len: 每张图片的大小
 * num: 图片的编号
 ************************/
void pic_display(char *fb, char *pic, int len, int num)
{
	memcpy(fb, (pic + (len*num)), len);
}


int main(int argc, char *argv[])
{
	int i, fd1, fd2, ret;
	struct fb_fix_screeninfo fix_info;
	struct fb_var_screeninfo var_info;
	int bpp, xres, yres, fb_len, pic_len, pic_num;
	void *fb_mmap, *pic_mmap;

	//1.打开/dev/fb0文件
	fd1 = open(DEF_FB, O_RDWR);
	if (fd1 < 0) {
		printf("open %s error\n", DEF_FB);
		goto err_open1;
	}
	printf("open %s\n", DEF_FB);

	//2.获得fb的信息并初始化相关变量
	ret = ioctl(fd1, FBIOGET_FSCREENINFO, &fix_info);
	if (ret) {
		printf("Get fix info error %d\n", ret);
		goto err_ioctl;
	}
	ret = ioctl(fd1, FBIOGET_VSCREENINFO, &var_info);
	if (ret) {
		printf("Get var info error: %d\n", ret);
		goto err_ioctl;
	}
	bpp = var_info.bits_per_pixel;
	xres = var_info.xres_virtual;
	yres = var_info.yres_virtual;
	//fb_len = fix_info.smem_len;
	fb_len = xres * yres * bpp / 8;
	printf("bpp:%d; xres:%d; yres:%d; fb_len %d\n", \
		bpp, xres, yres, fb_len);

	//3.对打开的fb设备文件进行mmap
	fb_mmap = mmap(NULL, fb_len, 
		PROT_READ | PROT_WRITE,
		MAP_SHARED, fd1, 0);
	if (fb_mmap == MAP_FAILED) {
		printf("mmap %s error\n", DEF_FB);
		goto err_mmap1;
	}

	//4.打开图片文件并计算大小和图片数量
	fd2 = open(argv[1], O_RDWR);
	if (fd2 == -1) {
		printf("open %s error\n", argv[1]);
		goto err_open2;
	}
	pic_len = lseek(fd2, 0, SEEK_END);
	pic_num = pic_len/(xres*yres*bpp/8);
	printf("open %s: %d bytes, %d pics\n", \
		argv[1], pic_len, pic_num);

	//5.对打开的图片文件执行mmap
	pic_mmap = mmap(NULL, pic_len, 
		PROT_READ | PROT_WRITE,
		MAP_SHARED, fd2, 0);
	if (fb_mmap == MAP_FAILED) {
		printf("mmap %s error\n", argv[1]);
		goto err_mmap2;
	}

	//6.循环显示文件中的图片
	for (i=0; i<10000; i++) {
		pic_display(fb_mmap, pic_mmap, fb_len, i%pic_num);
		printf("print pic %d\n", i%pic_num);
		sleep(2);
	}

	//7.取消映射并关闭文件
	munmap(fb_mmap, fb_len);
	munmap(pic_mmap, pic_len);
	close(fd2);
	close(fd1);

	return 0;

err_mmap2:
	close(fd2);
err_open2:
	munmap(fb_mmap, fb_len);
err_mmap1:
err_ioctl:
	close(fd1);
err_open1:
	exit(1);
}

