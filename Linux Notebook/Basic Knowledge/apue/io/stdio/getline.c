#include <stdio.h>

ssize_t mygetline(char **ptr, size_t *n, FILE *stream)
{


}

int main(void)
{
	char *ptr = NULL;
	size_t n = 0;
	ssize_t cnt = 0;

	while (1) {
		cnt = mygetline(&ptr, &n, stdin);
		//cnt = getline(&ptr, &n, stdin);
		if (cnt < 0)
			break;
		printf("n = %d, cnt = %d\n", n, cnt);
		puts(ptr);
	}

	return 0;
}

