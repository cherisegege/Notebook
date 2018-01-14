#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char **argv)
{
	DIR *dp;
	struct dirent *dent;

	if (argc < 2) {
		fprintf(stderr, "Usage....\n");		
		goto ERROR;	
	}
			
	dp = opendir(argv[1]);	
	if (NULL == dp) {
		perror("opendir()");
		goto ERROR;
	}

	errno = 0;
	while (1) {
		dent = readdir(dp);
		if (dent == NULL) {
			if (errno) {
				perror("readdir()");
				goto ERROR;
			}	
			break;
		}
		puts(dent->d_name);
	}

	rewinddir(dp);
	dent = readdir(dp);
	puts(dent->d_name);

	closedir(dp);

	return 0;
ERROR:
	return 1;
}

