/******************************
 * 读取并打印wave音乐的头部
 * $>./01wav_head 
 * 测试默认的wav文件
 * $>./01wav_head xxx.wav
 * 测试给定的wav文件
 * Author: zht
 * Date: 2015-05-26
 ******************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "wav_head.h"

#define DEF_FILE	"./music/1a.wav"

//将wave文件的格式转为字符串形式
char *wav_fmt_string(uint16_t fmt)
{
	switch (fmt) {
	case WAV_FMT_PCM:
		return "PCM";
		break;
	case WAV_FMT_IEEE_FLOAT:
		return "IEEE FLOAT";
		break;
	case WAV_FMT_DOLBY_AC3_SPDIF:
		return "DOLBY AC3 SPDIF";
		break;
	}

	return "NON Support Fmt";
}


//打印wave文件头
void wav_print_header(wav_container_t *container)
{
	printf("====打印WAV文件的文件头(共44字节)===\n");
	printf("<--WAV Header size %d bytes-->\n", 
		sizeof(container->header));
	printf("File Magic:         [%c%c%c%c]\n", 
		(char)(container->header.magic), 
		(char)(container->header.magic>>8), 
		(char)(container->header.magic>>16), 
		(char)(container->header.magic>>24));
	printf("File Length:        [%d]\n", \
		container->header.length);
	printf("File Type:          [%c%c%c%c]\n",
		(char)(container->header.type), 
		(char)(container->header.type>>8), 
		(char)(container->header.type>>16), 
		(char)(container->header.type>>24));
	printf("\n");

	printf("<--WAV FMT size %d bytes-->\n", 
		sizeof(container->format));
	printf("Fmt Magic:          [%c%c%c%c]\n",
		(char)(container->format.magic), 
		(char)(container->format.magic>>8), 
		(char)(container->format.magic>>16), 
		(char)(container->format.magic>>24));
	printf("Fmt Size:           [%d]\n", \
		container->format.fmt_size);
	printf("Fmt Format:         [0x%x-%s]\n", \
		container->format.format, \
		wav_fmt_string(container->format.format));
	printf("Fmt Channels:       [%d]\n", \
		container->format.channels);
	printf("Fmt Sample_rate:    [%d](HZ)\n", \
		container->format.sample_rate);
	printf("Fmt Bytes_p_second: [%d](Byte)\n", \
		container->format.bytes_p_second);
	printf("Fmt Blocks_align:   [%d]\n", \
		container->format.blocks_align);
	printf("Fmt Sample_length:  [%d](bit)\n", \
		container->format.sample_length);
	printf("\n");

	printf("<--WAV chunk header size %d bytes-->\n", 
		sizeof(container->chunk));
	printf("Chunk Type:         [%c%c%c%c]\n",
		(char)(container->chunk.type), 
		(char)(container->chunk.type>>8), 
		(char)(container->chunk.type>>16), 
		(char)(container->chunk.type>>24));
	printf("Chunk Length:       [%d](音频数据总长)\n\n", \
		container->chunk.length);
}


//检测wave文件的格式
int wav_check_valid(wav_container_t *container)
{
	if (container->header.magic != WAV_RIFF ||
		container->header.type != WAV_WAVE ||
		container->format.magic != WAV_FMT ||
		container->format.fmt_size != 16 ||
		(container->format.channels != 1 && container->format.channels != 2) ||
		container->chunk.type != WAV_DATA) {
		
		fprintf(stderr, "non standard wav file.\n");
		return -1;
	}

	return 0;
}


//读入wave文件的头
int wav_read_header(int fd, wav_container_t *con)
{
	//读入wav文件的头部
	if (read(fd, &con->header, sizeof(con->header)) 
		!= sizeof(con->header) || 
	read(fd, &con->format, sizeof(con->format)) 
		!= sizeof(con->format) ||
	read(fd, &con->chunk, sizeof(con->chunk)) 
		!= sizeof(con->chunk)) {
		fprintf(stderr, "Error wav_read_header\n");
		return -1;
	}

	if (wav_check_valid(con) < 0)
		return -1;

	//打印wave头信息
	wav_print_header(con);

	return 0;
}



int main(int argc, char *argv[])
{
	char *filename;
	char *devicename = "default";
	int fd;
	wav_container_t wav;
	
	if (argc == 1) {
		filename = DEF_FILE;
	}
	else if (argc == 2) {
		filename = argv[1];
	}
	else {
		fprintf(stderr, "Usage: ./01wav_head <FILE>\n");
		return -1;
	}
	
	//打开wave文件
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Error open [%s]\n", filename);
		return -1;
	}
	
	//读入并打印wave文件头信息
	if (wav_read_header(fd, &wav) < 0) {
		fprintf(stderr, "wav_parse error [%s]\n", filename);
		goto Err;
	}
	close(fd);
	return 0;

Err:
	close(fd);
	return -1;
}

