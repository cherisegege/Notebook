#include <stdio.h>
#include <unistd.h>		/*for _exit()*/
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf env;
static jmp_buf env1;

static int jmpjmp(int num)
{
	if(num <= 0){
		longjmp(env1, 1);
	}else if(num > 100)
	{
		longjmp(env1, 2);
	}

}


static int jmptest(int m, int n)
{
	int ret;

	if (!(n > 0 && n < 100)){
		if((ret = setjmp(env1)) == 0){
			jmpjmp(n);
		}else if((ret == 1)){
			printf("can shu xiao yu 0 le!\n");
			longjmp(env, 10);
		}else if(ret == 2){
			printf("can shu da yu 100 le!\n");
			longjmp(env, 20);
		}
	}else{ 
		return m % n;
	}
}

int main(int argc, char **argv)
{
	int ret;

	if (argc < 3) {	
		fprintf(stderr, "Usage...\n");
		exit(1);
	}

	if ((ret = setjmp(env)) == 0) {
		printf("the result is %d\n", jmptest(atoi(argv[1]), atoi(argv[2])));	
	} else if (ret == 10) {
		fprintf(stderr, "the third argument would be > 0\n");
	} else if (ret == 20) {
		fprintf(stderr, "the third argument would be < 100\n");
	} else
		_exit(1);

	exit(0);
}

