#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>

#define PATTERNSIZE	1024

static int ispoint(const char *path);
static long __mydu(const char *path);
static long mydu(const char *path);
int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage....\n");
		return 1;
	}

	printf("%ldK\n", mydu(argv[1]));

	return 0;
}

static long mydu(const char *path)
{
	return __mydu(path) / 2;	
}

//统计给定路径的文件所占st_blocks个数
static long __mydu(const char *path)
{
	struct stat statres;
	char pattern[PATTERNSIZE] = {};
	glob_t globres;
	long sum = 0;
	int i;
	
	if (lstat(path, &statres) < 0) {
		perror("lstat()");
		return -1;
	}

	if (!S_ISDIR(statres.st_mode))
		return (long)statres.st_blocks;

	//目录    "/etc"--->"/etc/*"
	strncpy(pattern, path, PATTERNSIZE);	
	strncat(pattern, "/*", PATTERNSIZE);
	glob(pattern, 0, NULL, &globres);

	//"/etc/.*"
	strncpy(pattern, path, PATTERNSIZE);	
	strncat(pattern, "/.*", PATTERNSIZE);
	glob(pattern, GLOB_APPEND, NULL, &globres);

	//目录本身大小
	sum = statres.st_blocks;
	for (i = 0; i < globres.gl_pathc; i++) {
		//"/etc/." "/etc/.."
		if (ispoint(globres.gl_pathv[i])) 
			sum += __mydu(globres.gl_pathv[i]);
	}

	return sum;
}

static int ispoint(const char *path)
{
	char *ret;

	ret = strrchr(path, '/');
	if (ret == NULL)
		return 1;
	if (!strcmp(ret+1, ".") || !strcmp(ret+1, ".."))
		return 0;

	return 1;
}




