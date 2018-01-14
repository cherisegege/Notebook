#include "fb.h"

static void do_resize(unsigned char *old_buf,int old_x,int old_y, 
		unsigned char *new_buf, int new_x,int new_y)
{
	unsigned char *old_buf_cur;
	unsigned char *new_buf_cur;
	int new_count;
	int old_count;
	int x;
	int y;

	new_buf_cur = new_buf;

	for(y = 0;y < new_y;y++)
	{
		old_buf_cur = old_buf + y * old_y / new_y * old_x * 3;

		new_count = 0;
		for(x = 0;x < new_x;x++)
		{
			old_count = x * old_x / new_x * 3;

			new_buf_cur[new_count + 0] = old_buf_cur[old_count + 0];
			new_buf_cur[new_count + 1] = old_buf_cur[old_count + 1];
			new_buf_cur[new_count + 2] = old_buf_cur[old_count + 2];
			new_count += 3;
		}
		new_buf_cur += new_x * 3;
	}
}

static void picture_resize(unsigned char **rgb_buf,
		int *x_pic,int *y_pic,
		int x_scr,int y_scr)
{
	int x_pic_old = *x_pic;
	int y_pic_old = *y_pic;

	if(x_pic_old > x_scr || y_pic_old > y_scr)
	{
		if(x_pic_old > x_scr)
			*x_pic = x_scr;
		else
			*x_pic = x_pic_old;
		
		if(y_pic_old > y_scr)
			*y_pic = y_scr;
		else
			*y_pic = y_pic_old;

		unsigned char *buf = malloc(*x_pic * *y_pic * 3);

		do_resize(*rgb_buf,x_pic_old,y_pic_old,buf,*x_pic,*y_pic);
		
		free(*rgb_buf);

		*rgb_buf = buf;
	}
}

int show_image(char *filename, char *out_filename)
{
	int x_pic = 0;
	int y_pic = 0;
	int x_scr = 0;
	int y_scr = 0;

	int x_scr_offs = 0;
	int y_scr_offs = 0;

	int ret = 0;

	unsigned char *rgb_buf = NULL;

	int (*load)(char *filename,unsigned char *rgb_buf,
			int x_pic,int y_pic);

	if(is_bmp(filename)&&(bmp_getsize(filename,&x_pic,&y_pic) == 0))
	{
		load = bmp_load;
		goto start;
	}

	if(is_jpeg(filename)&&(jpeg_getsize(filename,&x_pic,&y_pic) == 0))
	{
		load = jpeg_load;
		goto start;
	}

	fprintf(stderr,"%s: format unknown.\n",filename);
	return 1;
start:
	if((rgb_buf = malloc(x_pic * y_pic * 3)) == NULL)
	{
		fprintf(stderr,"%s: can't get memory.\n",filename);
		ret = 1;
		goto error;
	}

	if((load(filename,rgb_buf,x_pic,y_pic) != 0))	
	{
		fprintf(stderr,"%s: can't load image.\n",filename);
		ret = 1;
		goto error;
	}
	
	x_scr = X;
	y_scr = Y;
	
	if(x_pic < x_scr)	
		x_scr_offs = (x_scr - x_pic) / 2;
	else
		x_scr_offs = 0;

	if(y_pic < y_scr)	
		y_scr_offs = (y_scr - y_pic) / 2;
	else
		y_scr_offs = 0;

	picture_resize(&rgb_buf,&x_pic,&y_pic,x_scr,y_scr);
	
	mkimage(rgb_buf,x_pic,y_pic,x_scr_offs,y_scr_offs,out_filename);
		
error:
	return ret;
}


int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s src_file dis_file\n",
				argv[0]);
		exit(-1);
	}

	show_image(argv[1], argv[2]);
	
	return 0;
}





