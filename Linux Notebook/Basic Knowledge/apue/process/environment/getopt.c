#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int ret;

	while (1) {
		ret = getopt(argc, argv, "-la:b::");
		if (ret < 0)
			break;
		switch(ret) {
			case 1:
				printf("parse non-option argument: %s\n", argv[optind-1]);
				break;
			case 'l':
				printf("parse no-argument option: l\n");
				break;
			case 'a':
				printf("parse argument option: a\n");
				printf("the argument is: %s\n", optarg);
				break;
			case 'b':
				printf("parse optional-argument option:b\n");	
				if (optarg != NULL) {
					printf("the argument is: %s\n", optarg);
				}else {
					printf("have no argument\n");
				}
				break;
		}
	}


	return 0;
}

