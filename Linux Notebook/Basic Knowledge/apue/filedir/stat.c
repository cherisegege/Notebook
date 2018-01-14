#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	struct stat statres;

	if (argc < 2) {
		fprintf(stderr, "Usage...\n");
		goto ERROR;
	}

	if (lstat(argv[1], &statres) < 0) {
		perror("stat()");
		goto ERROR;
	}

	//if (S_ISREG(statres.st_mode))
	if ((statres.st_mode & S_IFMT) == S_IFREG)
		putchar('-');
	else if (S_ISDIR(statres.st_mode))
		putchar('d');
	else if ((statres.st_mode & S_IFMT) == S_IFLNK)
		putchar('l');
	else
		printf("臣妾做不到啊！！！\n");

	if (statres.st_mode & S_IRUSR)
		putchar('r');
	else
		putchar('-');
	if (statres.st_mode & S_IWUSR)
		putchar('w');
	else 
		putchar('-');
	if (statres.st_mode & S_IXUSR)
		putchar('x');
	else 
		putchar('-');

	printf(" %d ", statres.st_nlink);

	printf("%d %d", statres.st_uid, statres.st_gid);

	printf("  %d ", statres.st_size);

	printf("  %d", statres.st_mtime);

	putchar('\n');


	return 0;
ERROR:
	return 1;
}

