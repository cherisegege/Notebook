/*************************
 * 定义wav文件的头信息
 * Author: zht
 * Date: 2015-05-26
 *************************/
#ifndef __WAV_HEAD_H
#define __WAV_HEAD_H

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#define COMPOSE_ID(a,b,c,d)	\
((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))

#define WAV_RIFF	COMPOSE_ID('R','I','F','F')
#define WAV_WAVE	COMPOSE_ID('W','A','V','E')
#define WAV_FMT		COMPOSE_ID('f','m','t',' ')
#define WAV_DATA	COMPOSE_ID('d','a','t','a')

/* WAVE fmt block constants from Microsoft mmreg.h header */
#define WAV_FMT_PCM             0x0001
#define WAV_FMT_IEEE_FLOAT      0x0003
#define WAV_FMT_DOLBY_AC3_SPDIF 0x0092


typedef struct wav_header {
	uint32_t magic;		/* 'RIFF' */
	uint32_t length;	/* filelen */
	uint32_t type;		/* 'WAVE' */
} wav_header_t;

typedef struct wav_fmt {
	uint32_t magic;  	/* 'FMT '*/
	uint32_t fmt_size; 	/* 16 or 18 */
	uint16_t format;	/* see WAV_FMT_* */
	uint16_t channels;
	uint32_t sample_rate;	/* frequence of sample */
	uint32_t bytes_p_second;
	uint16_t blocks_align;	/* samplesize; 1 or 2 bytes */
	uint16_t sample_length;	/* 8, 12 or 16 bit */
} wav_fmt_t;

typedef struct wav_chunk_header {
	uint32_t type;		/* 'data' */
	uint32_t length;	/* samplecount */
} wav_chunk_header_t;

typedef struct wav_container {
	wav_header_t header;
	wav_fmt_t format;
	wav_chunk_header_t chunk;
} wav_container_t;

int wav_read_header(int fd, wav_container_t *container);

#endif /* #ifndef __WAV_HEAD_H */

