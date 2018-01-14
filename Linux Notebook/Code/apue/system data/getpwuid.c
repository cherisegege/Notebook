#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char **argv)
{
	struct stat statres;
	struct passwd *pw;
	struct group *gp;

	if (argc < 2)
		return 1;	

	lstat(argv[1], &statres);	
	//if error

	pw = getpwuid(statres.st_uid);
	printf("owner: %s\n", pw->pw_name);

	gp = getgrgid(statres.st_gid);
	printf("group: %s\n", gp->gr_name);

	return 0;
}

