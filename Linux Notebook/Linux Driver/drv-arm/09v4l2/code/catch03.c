/*****************************
 * jpeg格式图片的捕获
 * 对catch02的例子进一步整理了一下
 * 首先将YUV422转换为YUV420P，然后存储为jpeg图片
 * 用法：
 * $>catch03
 * $>catch03 /dev/video0
 * YUV422在内存中应该是这样的(每个点占2字节)：
 * YUYV YUYV YUYV YUYV...
 * YUYV YUYV YUYV YUYV...
 * YUV420P是这样的(8个点一共占12字节)：
 * YYYYYYYY
 * UUVV
 * Author: zht
 * Date: 2016-08-12
 *****************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <jpeglib.h>
#include <jerror.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#define DEF_DEV "/dev/video0"
//#define DEF_DIR	"/storage/sd_external/media/"
#define DEF_DIR "./pics"

#define WIDTH			640
#define HEIGHT			480
#define QUALITY			100
#define NAME_SIZE		30
#define FILE_SIZE		100
#define MMAP_BUFFERS	2
#define FRAME_CNT		20

struct v4l2_capability cap;
enum v4l2_buf_type type;
struct v4l2_requestbuffers req;
struct v4l2_buffer buf;
char dev_name[NAME_SIZE+1] = {0};
char file_name[FILE_SIZE+1] = {0};
char *yuv422_buf;  //存储yuv422格式的图片
char *yuv420p_buf;  //存储yuv420p格式的图片

struct buf_info {
	int size;
	char *ptr;
};
struct buf_info buffers[MMAP_BUFFERS];


/***********************************
 * 将yuv422图片转换为yuv420p的
 * yuv422将三个分量混合存储，而yuv420p在内存中独立存储三个分量
 * Y在最前面，后面是U和V
 * map: 指向yuv420p的缓冲区
 * cap_map: 指向yuv422的缓冲区
 ***********************************/
void yuv422to420p(char *map, char *cap_map, 
		int width, int height)
{
    char *src, *dest, *src2, *dest2;
    int i, j;

    //创建Y分量(把图片中所有的Y拷贝到一起)
    src = cap_map;
    dest = map;
    for (i = width * height; i > 0; i--) {
        *dest++ = *src;
        src += 2;
    }

    //创建U和V分量
    //src指向0行的第一个U，src2指向1行的第一个U
    //!不能将两个U平均，会有噪声，取一行即可
    //dest是U分量的起始地址，des2为V分量
    src = cap_map + 1;
    src2 = cap_map + width * 2 + 1;
    dest = map + width * height;
    dest2 = dest + (width * height) / 4;
    for (i = height / 2; i > 0; i--) {
        for (j = width / 2; j > 0; j--) {
            //*dest = ((int) *src + (int) *src2) / 2;
			*dest = (int)*src;
			src += 2;
            src2 += 2;
            dest++;
            //*dest2 = ((int) *src + (int) *src2) / 2;
            *dest2 = (int)*src2;
            src += 2;
            src2 += 2;
            dest2++;
        }
        src += width * 2;
        src2 += width * 2;
    }
}


/**********************************
 * 将yuv420p格式的图片存储为jpeg文件
 * fp: 打开的文件
 * image: yuv420p图片的缓冲区
 * quality: 图片的质量，默认为75
 **********************************/
static void 
save_jpeg_file(FILE *fp, unsigned char *image, int width, 
            int height, int quality)
{
    int i,j;

    JSAMPROW y[16],cb[16],cr[16]; // y[2][5] = color sample of row 2 and pixel column 5; (one plane)
    JSAMPARRAY data[3]; // t[0][2][5] = color sample 0 of row 2 and column 5

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    data[0] = y;
    data[1] = cb;
    data[2] = cr;

    cinfo.err = jpeg_std_error(&jerr);  // errors get written to stderr 
    
    jpeg_create_compress(&cinfo);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    jpeg_set_defaults(&cinfo);

    jpeg_set_colorspace(&cinfo, JCS_YCbCr);

    cinfo.raw_data_in = TRUE; // supply downsampled data
	//注意！下面这一句在用gcc编译时需要注释掉，但用arm-linux-gcc编译时必须打开，否则会出现段错误
	//cinfo.do_fancy_downsampling = FALSE;  // fix segfaulst with v7
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 2;
    cinfo.comp_info[1].h_samp_factor = 1;
    cinfo.comp_info[1].v_samp_factor = 1;
    cinfo.comp_info[2].h_samp_factor = 1;
    cinfo.comp_info[2].v_samp_factor = 1;

    jpeg_set_quality(&cinfo, quality, TRUE);
    cinfo.dct_method = JDCT_FASTEST;

    jpeg_stdio_dest(&cinfo, fp);        // data written to file
    jpeg_start_compress(&cinfo, TRUE);

    for (j = 0; j < height; j += 16) {
        for (i = 0; i < 16; i++) {
            y[i] = image + width * (i + j);
            if (i % 2 == 0) {
                cb[i / 2] = image + width * height + width / 2 * ((i + j) / 2);
                cr[i / 2] = image + width * height + width * height / 4 + width / 2 * ((i + j) /2);
            }
        }
        jpeg_write_raw_data(&cinfo, data, 16);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
}


/******************************
 * 将缓冲区的内容存入jpeg文件
 * yuv422: 原始图片
 * yuv420p: 转换过的图片
 * num: 图片编号
 ******************************/
int save_to_file(char *yuv422, char *yuv420p, 
	int width, int height, int num)
{
	FILE *fp, ret;

	//转换图片格式
	yuv422to420p(yuv420p, yuv422, width, height);

	//创建新文件
	snprintf(file_name, FILE_SIZE, "%s/pic%d.jpg", \
		DEF_DIR, num);
	fp = fopen(file_name, "w+");
	if (!fp) {
		printf("Create file %s error\n", file_name);
		return -1;
	}

	//将数据写入文件
	save_jpeg_file(fp, yuv420p, width, height, QUALITY);
	fclose(fp);	
	return 0;
}


int main(int argc, char *argv[])
{
	int fd, ret, i, size422, size420p;

	if (argc == 1) {
		snprintf(dev_name, NAME_SIZE, DEF_DEV);
	}
	else if (argc == 2) {
		snprintf(dev_name, NAME_SIZE, argv[1]);
	}
	else {
		printf("Use $>catch03 /dev/xxx\n");
		exit(1);
	}

	//打开视频文件
	fd = open(dev_name, O_RDWR);
	if (fd == -1) {
		fprintf(stderr, "Cannot open %s\n", dev_name);
		exit(1);
	}
	printf("1. open %s 成功\n", dev_name);

	//测试设备是否支持流式访问
	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if (ret < 0) {
		printf("Cannot support VIDIOC_QUERYCAP\n");
		goto out;
	}
	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		printf("Not stream device, cannot mmap\n");
		goto out;
	}
	printf("2.执行QUERYCAP命令，测试设备是否支持流式访问\n");

	//执行命令VIDIOC_REQBUFS(v4l2_requestbuffers)
	req.count = MMAP_BUFFERS;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	ret = ioctl(fd, VIDIOC_REQBUFS, &req);
	if (ret == -1) {
		printf("Error requesting buffers %d for memory map. VIDIOC_REQBUFS", req.count);
		goto out;
	}
	if (req.count < MMAP_BUFFERS) {
		printf("Insufficient buffer memory.");
		goto out;
	}
	printf("3. 执行REQBUFS命令，Mmap %d frames\n", req.count);
	printf("\n============================\n\n");

	//为每个frame执行mmap
	//执行命令VIDIOC_QUERYBUF以及VIDIOC_QBUF
	printf("4. 分别对%d个frame执行QUERYBUF命令，mmap\n", req.count);
	for (i = 0; i < req.count; i++) {
		memset(&buf, 0, sizeof(struct v4l2_buffer));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		ret = ioctl(fd, VIDIOC_QUERYBUF, &buf);
		if (ret == -1) {
			printf("Error querying buffer %d (VIDIOC_QUERYBUF)", i);
			goto out;
		}
		printf("frame %d: 执行QUERYBUF命令\n", i);

		buffers[i].size = buf.length;
		buffers[i].ptr = mmap(NULL, buf.length, \
				PROT_READ | PROT_WRITE, \
				MAP_SHARED, \
				fd, buf.m.offset);
		if (buffers[i].ptr == MAP_FAILED) {
			printf("Error mapping buffer %i", i);
			goto out;
		}
		printf("frame %d: mmap到%p(offset %d), length %d\n", \
			i, buffers[i].ptr, buf.m.offset, buffers[i].size);
	}

	//执行命令VIDIOC_STREAMON
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if (ret == -1) {
		printf("Stream on error (VIDIOC_STREAMON)\n");

		goto out;
	}
	printf("5. 执行STREAMON命令，启动流\n");
	printf("===========================\n\n");

	//分配图片缓冲区
	size422 = WIDTH * HEIGHT * 2;
	yuv422_buf = (char *)malloc(size422);
	size420p = WIDTH * HEIGHT * 3 / 2;
	yuv420p_buf = (char *)malloc(size420p);
	if (!yuv422_buf || !yuv420p_buf) {
		printf("Sufficient memory\n");
		goto out;
	}
	printf("6. 为图片分配两个缓冲区，分别存422/420p的数据\n");
	printf("分别为%p(%dB), %p(%dB)\n", \
		yuv422_buf, size422, yuv420p_buf, size420p);

	//从video设备获得frame
	for (i = 0; i < FRAME_CNT; i++) {
		memset(yuv422_buf, 0, size422);
		memset(&buf, 0, sizeof(struct v4l2_buffer));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i % MMAP_BUFFERS;

		ret = ioctl(fd, VIDIOC_QBUF, &buf);
		if (ret == -1) {
			printf("VIDIOC_QBUF error\n");
			//continue;
		}
		printf("frame %d: 执行QBUF命令加入队列\n", i);

		ret = ioctl(fd, VIDIOC_DQBUF, &buf);
		if (ret == -1) {
			printf("VIDIOC_DQBUF error\n");
			continue;
		}
		printf("frame %d: 执行DQBUF命令，等待直到数据返回\n", i);

		memcpy(yuv422_buf, buffers[buf.index].ptr, size422);
		printf("copy buffer OK\n");
		ret = save_to_file(yuv422_buf, yuv420p_buf, \
			WIDTH, HEIGHT, i);
		if (ret) {
			printf("Cannot save to file\n");
		}
		//sleep(1);
	}

	free(yuv420p_buf);
	free(yuv422_buf);
	close(fd);
	return 0;
out:	
	free(yuv420p_buf);
	free(yuv422_buf);
	close(fd);
	exit(1);
}


