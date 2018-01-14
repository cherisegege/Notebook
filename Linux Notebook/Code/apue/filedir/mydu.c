#include <stdio.h>
#include <glob.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define PARTEN	"/etc/*"
#define BUFSIZE 1024


static char file_type(mode_t mode);
long caculate(const char *parten);
int ispoint(char *path);

int main(int argc, char *argv[])
{
	glob_t *pg;
	int i;
	int ret;
	struct stat buf_st;


    printf("%ld\n", caculate(argv[1]) / 2);

	return 0;
}

static char file_type(mode_t mode)
{
	mode &= S_IFMT;
	switch(mode){
		case S_IFSOCK:	return 's';	break;
		case S_IFLNK:	return 'l';	break;
		case S_IFREG:	return '-';	break;
		case S_IFBLK:	return 'b';	break;
		case S_IFDIR:	return 'd';	break;
		case S_IFCHR:	return 'c';	break;
		case S_IFIFO:	return 'p';	break;
		default:		return ' '; break;
	}
}

long caculate(const char *parten)
{
	long size = 0;
	glob_t pg;
	int i;
	int ret;
	struct stat buf_st;
	char buf[BUFSIZE];

	ret = lstat(parten, &buf_st);
	if(ret < 0){
		perror("stat()");
		return 1;
	}
	//是文件直接返回
	if(file_type(buf_st.st_mode) == '-'){
		return (long)buf_st.st_blocks;
	}
	//是目录,处理目录格式
	//加上/*   和  加上/.*
	strncpy(buf, parten, BUFSIZE);
	strncat(buf, "/*", BUFSIZE);
	glob(buf, 0, NULL, &pg);

	strncpy(buf, parten, BUFSIZE);
	strncat(buf, "/.*", BUFSIZE);
	glob(buf, GLOB_APPEND, NULL, &pg);
	//加上自己大小，再继续遍历
	size += buf_st.st_blocks;
	//递归遍历所有的内容
	for(i = 0; i < pg.gl_pathc; i++){
		if(ispoint(pg.gl_pathv[i])){
			size += caculate(pg.gl_pathv[i]);
		}
	}
	globfree(&pg);

	return size;
}

int ispoint(char *path)
{
	char *p;
	p = strrchr(path, '/');

	if(p == NULL){
		return 1;
	}
	if(!strcmp(p+1, ".") || !strcmp(p+1, "..")){
		return 0;
	}
	return 1;
}

