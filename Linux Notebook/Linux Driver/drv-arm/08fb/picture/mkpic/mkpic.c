#include "fb.h"

#define ERROR(condition,msg)	\
	if(condition)			\
{				\
	fprintf(stderr,"%s: %d: %s: %s\n",__FILE__,__LINE__,msg,strerror(errno));	\
	exit(-1);		\
}


void mkimage(unsigned char *rgb_buf,
		int x_pic, int y_pic, 
		int x_scr_offs, int y_scr_offs, char *out_filename)
{
	unsigned short *image16_buf = NULL;
	unsigned int *image32_buf = NULL;
	unsigned char *image_buf = NULL;
	unsigned char *image_buf_cur = NULL;

	unsigned char *fb_buf = NULL;
	unsigned char *fb_buf_cur = NULL;

	int fd;
	int i;	

	int x_scr;
	int y_scr;
	int screen_pixel;
	int pic_pixel;
	int x_min;
	int y_min;
	int cpp;

	fd = open(out_filename,O_RDWR | O_CREAT | O_TRUNC, 0644);
	ERROR(fd == -1,"open()");

	if(BPP == 24)
	{
		cpp = 4;
	}
	else if(BPP == 16)
	{
		cpp = 2;
	}
	else if(BPP==32)
{
	cpp=6;
}
	else
	{
		printf("BPP is not 16 or 24...\n");
		exit(-1);
	}

	ftruncate(fd, X * Y * cpp);

	x_scr = X;
	y_scr = Y;
	screen_pixel = x_scr * y_scr;
	pic_pixel = x_pic * y_pic;

	
	image_buf = calloc(pic_pixel,cpp);

	switch(BPP)
	{
		case 16:
		image16_buf = (void *)image_buf;
		for(i = 0;i < pic_pixel;i++)
		{
		unsigned char r = rgb_buf[i * 3 + 0];
		unsigned char g = rgb_buf[i * 3 + 1];
		unsigned char b = rgb_buf[i * 3 + 2];

		image16_buf[i] = (((r >> 3) & 0x1f) << 11) | 
			(((g >> 2) & 0x3f) << 5) | 
			((b >> 3) & 0x1f);
		}
		break;
		case 24:
		image32_buf = (void *)image_buf;
		for(i = 0;i < pic_pixel;i++)
		{
		unsigned char r = rgb_buf[i * 3 + 0];
		unsigned char g = rgb_buf[i * 3 + 1];
		unsigned char b = rgb_buf[i * 3 + 2];				image32_buf[i] = (r << 16) | (g << 8) | b;
		}
		break;
	}
	
	if(x_scr_offs + x_pic > x_scr)
		x_scr_offs = 0;
	if(y_scr_offs + y_pic > y_scr)
		y_scr_offs = 0;

	if(x_pic < x_scr)
		x_min = x_pic;
	else
		x_min = x_scr;

	if(y_pic < y_scr)
		y_min = y_pic;
	else
		y_min = y_scr;

	fb_buf = mmap(NULL,screen_pixel * cpp,PROT_WRITE | PROT_READ,MAP_SHARED,fd,0);
	ERROR(fb_buf == MAP_FAILED,"mmap()");

	fb_buf_cur = fb_buf + (y_scr_offs * x_scr + x_scr_offs) * cpp;

	image_buf_cur = (void *)image_buf;

	for(i = 0; i < y_min; i++)
	{
		memcpy(fb_buf_cur,image_buf_cur,x_min * cpp);
		fb_buf_cur += x_scr * cpp;
		image_buf_cur += x_pic * cpp;
	}	

	munmap(fb_buf,screen_pixel * cpp);
	free(image_buf);
	close(fd);
}

