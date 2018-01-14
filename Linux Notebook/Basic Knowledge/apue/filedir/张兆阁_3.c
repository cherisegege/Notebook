#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define BUFSIZE 1024

static char file_type(mode_t mode);
static int file_permission(mode_t mode);
static int file_info(struct stat buf_st);

int main(int argc, char *argv[])
{
	int ret;
	char per[10] = {};
	struct stat buf_st;
	DIR *dir;
	struct dirent *dirent;
	char direct[100] = {};
	
	if(argc < 2){
		printf("参数太少!\n");
		return -1;
	}
	dir = opendir(argv[1]);
	while(1){
		dirent = readdir(dir);
		if(dirent == NULL)
			break;
		direct[0] = '\0';
		strcat(direct, argv[1]);
		strcat(direct, dirent->d_name);
		ret = stat(direct, &buf_st);
		if(ret < 0){
			perror("stat()");
			return 1;
		}
		putchar(file_type(buf_st.st_mode));
		file_permission(buf_st.st_mode);
		file_info(buf_st);
		printf(" %s\n", direct);
	}
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

static int file_permission(mode_t mode)
{
	int i = 0;
	unsigned int flag = 256, flag1 = 4096;

	for(i = 9; i > 0; i--){
		if(mode & flag){
			if((i % 3) == 0)
				putchar('r');
			if((i % 3) == 2)
				putchar('w');
			if((i % 3) == 1){
				if((mode & flag1) && (i == 1)){
					putchar('t');
				}else if(mode & flag1){
					putchar('s');
				}else if(mode & flag1){
					putchar('s');
				}else{
					putchar('x');
				}
			}
		}else{
			if((mode & flag1) && (i == 1)){
				putchar('T');
			}else if((mode & flag1) && (i == 4)){
				putchar('S');
			}
			else if((mode & flag1) && (i == 7)){
				putchar('S');
			}else
				putchar('-');
		}
		flag = flag >> 1;
		if(i % 3 == 2)
		flag1 = flag1 >> 1;
	}
	return 0;
}

static int file_info(struct stat buf_st)
{
	struct passwd *pw;
	struct group *gr;
	struct tm *tmp;
	char buf[BUFSIZE];

	putchar('.');
	printf(" %-2d", buf_st.st_nlink);
	pw = getpwuid(buf_st.st_uid);
	fprintf(stdout, "%-6s", pw->pw_name);
	gr = getgrgid(buf_st.st_gid);
	fprintf(stdout, "%-6s", gr->gr_name);
	printf(" %-8d", buf_st.st_size);
	tmp = localtime(&buf_st.st_mtime);
	if(NULL == tmp){
		fprintf(stderr, "localtime() failed!!!\n");
		return -1;
	}
	strftime(buf, BUFSIZE, "%2m月 %2d %2H:%2M", tmp);
	fprintf(stdout, "%s", buf);

	return 0;
}

