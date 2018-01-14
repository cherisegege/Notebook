#include <stdio.h>
#include <stdlib.h>

#define MALLOCNUM	120 

ssize_t mygetline(char **ptr, size_t *n, FILE *stream)
{
	size_t temp;
	char *new = NULL;
	ssize_t cnt = 0;
	int i = 0;

	if(*ptr == NULL){
		*ptr = malloc(MALLOCNUM);
		if(*ptr == NULL){
			printf("ERROR: mallco failed!\n");
			return 1;
		}
		*n += MALLOCNUM;
	}
	temp = *n;
	while((*(*ptr+(i++)) = fgetc(stream)) != '\n'){
		cnt++;
		if(--temp == 0){
			new = realloc(*ptr, *n += MALLOCNUM);
			if(new == NULL){
				printf("ERROR: reallco failed!\n");
			 	return 2;
			}
			*ptr = new;
			temp = MALLOCNUM;	
		}
	}

	return cnt;
}

int main(void)
{
	char *ptr = NULL;
	size_t n = 0;
	ssize_t cnt = 0;

	while (1) {
	//	cnt = mygetline(&ptr, &n, stdin);
		cnt = getline(&ptr, &n, stdin);
		if (cnt < 0)
			break;
		printf("n = %d, cnt = %d\n", n, cnt);
		puts(ptr);
	}

	return 0;
}

