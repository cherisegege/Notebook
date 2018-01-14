#include <stdio.h>
#include <glob.h>

#define PATTERN		"/etc/*"
#define PATTERN2	"/etc/.*"

int main(void)
{
	glob_t globres;
	int i;

	glob(PATTERN, GLOB_NOSORT, NULL, &globres);
	glob(PATTERN2, GLOB_APPEND | GLOB_NOSORT, NULL, &globres);

	for (i = 0; i < globres.gl_pathc; i++) {
		puts(globres.gl_pathv[i]);
	}	

	globfree(&globres);

	return 0;
}


