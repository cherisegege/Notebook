#include <stdio.h> /*for printf*/
#include <stdlib.h> /*for exit*/
#include <getopt.h> /*for getopt_long*/

int main(int argc, char **argv)
{
	int ret, longindex = 0;
	int c;

	struct option arr[] = {
		{"help", no_argument, NULL, 100},
		{"list", required_argument, &ret, 's'},
		{"tired", optional_argument, NULL, 't'},
		{0,0,0,0}
	};

	while (1) {
		c = getopt_long(argc, argv, "la:", arr, &longindex);
		if (c < 0)
			break;
		switch(c) {
			case 'l':
				printf("parse option l\n");
				break;
			case 'a':
				printf("parse option a, the argument is %s\n", optarg);
				break;
			case 100:
				printf("parse long option %s\n", arr[longindex].name);
				break;
			case 0:
				if (ret == 's') {
					printf("parse long option %s\n", arr[longindex].name);
					printf("the argument: %s\n", optarg);
				}
				break;
			case 't':
				printf("parse long option %s\n", arr[longindex].name);
				if (optarg != NULL)
					printf("the argument is %s\n", optarg);
				break;
			default:
				break;
		}
	}


	exit(EXIT_SUCCESS);
}

