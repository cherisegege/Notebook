/******************************
 * wave格式音乐的播放
 * 基于alas库，可以完整播放一首wav歌曲
 * 只能播放PCM格式的，其他格式不支持
 * Author: zht
 * Date: 2016-08-12
 ******************************/
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <alsa/asoundlib.h>
#include "wav_play.h"

#define DEF_FILE	"./music/1a.wav"


//读入wave文件的头部
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

	return 0;
}


//从wave文件的头部获得音频的格式
int sndwav_get_format(wav_container_t *wav, 
		snd_pcm_format_t *snd_format)
{	
	if (wav->format.format != WAV_FMT_PCM) {
		printf("wav->format.format 0x%x\n", wav->format.format);
		return -1;
	}
	
	switch (wav->format.sample_length) {
	case 16:
		*snd_format = SND_PCM_FORMAT_S16_LE;
		break;
	case 8:
		*snd_format = SND_PCM_FORMAT_U8;
		break;
	default:
		*snd_format = SND_PCM_FORMAT_UNKNOWN;
		break;
	}

	return 0;
}


//设置访问音频播放设备所需的参数
int sndwav_set_params(snd_pcm_container_t *sndpcm, 
		wav_container_t *wav)
{
	snd_pcm_hw_params_t *hwparams;
	snd_pcm_format_t format;
	uint32_t exact_rate;
	uint32_t buffer_time, period_time;

	/* 在栈上分配snd_pcm_hw_params_t */
	snd_pcm_hw_params_alloca(&hwparams);
	
	/* 初始化hwparams */
	if (snd_pcm_hw_params_any(sndpcm->handle, hwparams) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params_any\n");
		goto ERR_SET_PARAMS;
	}

	if (snd_pcm_hw_params_set_access(sndpcm->handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params_set_access\n");
		goto ERR_SET_PARAMS;
	}

	/* 设置采样的格式 */
	if (sndwav_get_format(wav, &format) < 0) {
		fprintf(stderr, "Error get_snd_pcm_format\n");
		goto ERR_SET_PARAMS;
	}
	if (snd_pcm_hw_params_set_format(sndpcm->handle, hwparams, format) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params_set_format\n");
		goto ERR_SET_PARAMS;
	}
	sndpcm->format = format;

	/* 设置通道数量 */
	if (snd_pcm_hw_params_set_channels(sndpcm->handle, hwparams, wav->format.channels) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params_set_channels\n");
		goto ERR_SET_PARAMS;
	}
	sndpcm->channels = wav->format.channels;

	/* 设置采样率，如果设备不支持，则使用最接近的采样率 */
	exact_rate = wav->format.sample_rate;
	if (snd_pcm_hw_params_set_rate_near(sndpcm->handle, hwparams, &exact_rate, 0) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params_set_rate_near\n");
		goto ERR_SET_PARAMS;
	}
	if (wav->format.sample_rate != exact_rate) {
		fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n ==> Using %d Hz instead.\n", 
			wav->format.sample_rate, exact_rate);
	}

	//获取设备支持的最大缓冲时间
	if (snd_pcm_hw_params_get_buffer_time_max(hwparams, &buffer_time, 0) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params_get_buffer_time_max\n");
		goto ERR_SET_PARAMS;
	}
	printf("max buffer_time is %fs, shrink to 1s\n", \
		(double)buffer_time/1000000);
	if (buffer_time > 1000000) 
		buffer_time = 1000000;
	period_time = buffer_time / 4;

	//设置buffer时间和peroid时间
	//默认设置为1秒和0.25秒
	if (snd_pcm_hw_params_set_buffer_time_near(sndpcm->handle, hwparams, &buffer_time, 0) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params_set_buffer_time_near\n");
		goto ERR_SET_PARAMS;
	}

	if (snd_pcm_hw_params_set_period_time_near(sndpcm->handle, hwparams, &period_time, 0) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params_set_period_time_near\n");
		goto ERR_SET_PARAMS;
	}

	/* 设置hw params */
	if (snd_pcm_hw_params(sndpcm->handle, hwparams) < 0) {
		fprintf(stderr, "Error snd_pcm_hw_params(handle, params)\n");
		goto ERR_SET_PARAMS;
	}

	snd_pcm_hw_params_get_period_size(hwparams, &sndpcm->chunk_size, 0);	
	snd_pcm_hw_params_get_buffer_size(hwparams, &sndpcm->buffer_size);
	if (sndpcm->chunk_size == sndpcm->buffer_size) {		
		fprintf(stderr, ("Can't use period equal to buffer size (%lu == %lu)\n"), sndpcm->chunk_size, sndpcm->buffer_size);		
		goto ERR_SET_PARAMS;
	}

	printf("buffer_size is %d frames(1S);\n", \
		sndpcm->buffer_size);
	printf("chunk_size is %d frames(1S);\n", \
		sndpcm->chunk_size);

	sndpcm->bits_per_sample = snd_pcm_format_physical_width(format);
	sndpcm->bits_per_frame = sndpcm->bits_per_sample * wav->format.channels;
	
	sndpcm->chunk_bytes = sndpcm->chunk_size * sndpcm->bits_per_frame / 8;
	printf("bits_per_sample is %dbits;\n", \
		sndpcm->bits_per_sample);
	printf("bits_per_frame is %dbits;\n", \
		sndpcm->bits_per_frame);
	printf("chunk_bytes is %dB(%d frames);\n", \
		sndpcm->chunk_bytes, sndpcm->chunk_size);

	//分配音频缓冲区，可播放0.5秒声音数据的缓冲区
	sndpcm->data_buf = (uint8_t *)malloc(sndpcm->chunk_bytes);
	if (!sndpcm->data_buf) {
		fprintf(stderr, "Error malloc %dB\n", sndpcm->chunk_bytes);
		goto ERR_SET_PARAMS;
	}

	return 0;

ERR_SET_PARAMS:
	return -1;
}


//将音频数据写入snd_pcm_t设备
//参数wcount是缓冲中的frame数量
ssize_t 
sndwav_write_pcm(snd_pcm_container_t *sndpcm, size_t wcount)
{
	ssize_t r;
	ssize_t result = 0;
	uint8_t *data = sndpcm->data_buf;

	if (wcount < sndpcm->chunk_size) {
		snd_pcm_format_set_silence(sndpcm->format, 
			data + wcount * sndpcm->bits_per_frame / 8, 
			(sndpcm->chunk_size - wcount) * sndpcm->channels);
		wcount = sndpcm->chunk_size;
	}

	while (wcount > 0) {
		r = snd_pcm_writei(sndpcm->handle, data, wcount);
		if (r == -EAGAIN || (r >= 0 && r < wcount)) {
			snd_pcm_wait(sndpcm->handle, 1000);
		} else if (r == -EPIPE) {
			snd_pcm_prepare(sndpcm->handle);
			fprintf(stderr, "<<<<< Buffer Underrun >>>>>\n");
		} else if (r == -ESTRPIPE) {			
			fprintf(stderr, "<<<<< Need suspend >>>>>\n");		
		} else if (r < 0) {
			fprintf(stderr, "Error snd_pcm_writei: [%s]\n", snd_strerror(r));
			exit(1);
		}
		if (r > 0) {
			result += r;
			wcount -= r;
			data += r * sndpcm->bits_per_frame / 8;
		}
	}
	return result;
}


//从fd中读count个字节到buf中
//返回值为读出的字节数，错误返回负数
ssize_t 
sndwav_safe_read(int fd, void *buf, size_t count)
{
	ssize_t result = 0, res;

	while (count > 0) {
		if ((res = read(fd, buf, count)) == 0)
			break;
		if (res < 0)
			return result > 0 ? result : res;
		count -= res;
		result += res;
		buf = (char *)buf + res;
	}
	return result;
}


//播放音乐
//fd是打开的wav音乐文件，sndpcm中包含要播放的音乐设备
//首先从wav文件中读入音频信息，写入sndpcm->data_buf
//然后将data_buf中的信息写入播放设备
//wav的chunk.length应该远远大于播放缓冲区的大小
void sndwav_play(snd_pcm_container_t *sndpcm, 
		wav_container_t *wav, int fd)
{
	int load, ret;
	uint32_t written = 0;
	uint32_t c;
	uint32_t count = wav->chunk.length;

	//count中是整个wav文件音频数据的大小
	//在while循环中播放整首歌曲
	load = 0;
	while (written < count) {
		//每次应读入chunk_bytes个字节
		//默认为0.5秒的音频数据(4字节/frame)
		do {
			c = count - written;
			if (c > sndpcm->chunk_bytes)
				c = sndpcm->chunk_bytes;
			c -= load;

			if (c == 0)
				break;
			ret = sndwav_safe_read(fd, sndpcm->data_buf + load, c);
			if (ret < 0) {
				fprintf(stderr, "Error safe_read/n");
				exit(1);
			}
			if (ret == 0)
				break;
			load += ret;
		} while ((size_t)load < sndpcm->chunk_bytes);

		//load为从文件读入的字节数，将其转为frame数
		load = load * 8 / sndpcm->bits_per_frame;

		//将缓冲中的数据写入音频播放设备，单位为frame数
		ret = sndwav_write_pcm(sndpcm, load);
		if (ret != load)
			break;
		
		ret = ret * sndpcm->bits_per_frame / 8;
		written += ret;
		load = 0;
	}
}



int main(int argc, char *argv[])
{
	char *filename;
	char *devicename = "default";
	int fd;
	wav_container_t wav;
	snd_pcm_container_t playback;

	if (argc == 1) {
		filename = DEF_FILE;
	}
	else if (argc == 2) {
		filename = argv[1];
	}
	else {
		fprintf(stderr, "Usage: ./02wav_play <FILE>\n");
		return -1;
	}
	
	memset(&playback, 0x0, sizeof(playback));

	//打开wave文件
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Error open [%s]\n", filename);
		return -1;
	}
	
	//读入wave文件头
	if (wav_read_header(fd, &wav) < 0) {
		fprintf(stderr, "Error wav_read_header [%s]\n", filename);
		goto Err;
	}
	fflush(stdout);

	//将音频信息的输出绑定到标准输出
	if (snd_output_stdio_attach(&playback.log, stderr, 0) < 0) {
		fprintf(stderr, "Error snd_output_stdio_attach\n");
		goto Err;
	}

	//打开播放设备
	if (snd_pcm_open(&playback.handle, devicename, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
		fprintf(stderr, "Error snd_pcm_open [ %s]\n", devicename);
		goto Err;
	}

	//设置播放设备
	if (sndwav_set_params(&playback, &wav) < 0) {
		fprintf(stderr, "Error set_snd_pcm_params\n");
		goto Err;
	}

	//输出音频信息
	snd_pcm_dump(playback.handle, playback.log);

	sndwav_play(&playback, &wav, fd);

	snd_pcm_drain(playback.handle);

	close(fd);
	free(playback.data_buf);
	snd_output_close(playback.log);
	snd_pcm_close(playback.handle);
	return 0;

Err:
	close(fd);
	if (playback.data_buf) 
		free(playback.data_buf);
	if (playback.log) 
		snd_output_close(playback.log);
	if (playback.handle) 
		snd_pcm_close(playback.handle);
	return -1;
}

