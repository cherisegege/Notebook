#include <stdio.h>

int main(void)
{
	char buf[20] = {};

	//setbuf(stdout, NULL);	
	
	//setvbuf(stdout, NULL, _IONBF, 0);

	setvbuf(stdout, buf, _IOFBF, 5);

	while (1) {
		printf("helloa");
		usleep(500000);
	}

	return 0;
}

