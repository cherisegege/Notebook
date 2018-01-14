#include <stdio.h>
#include <string.h>

#define NAMESIZE	20
#define BUFSIZE		1024

int jiafa(int a, int b);
int jianfa(int a, int b);
int chengfa(int a, int b);
int chufa(int a, int b);
int parse(char *buf, int *argc, char **argv);
struct op_st{
	char name[NAMESIZE];
	int(*pf)(int, int);
	char opch;
};
struct op_st arr[4] = {
	{"add", jiafa, '+'},
	{"sub", jianfa, '-'},
	{"mul", chengfa, '*'},
	{"div", chufa, '/'}
};

int jiafa(int a, int b)
{
	return a + b;
}

int jianfa(int a, int b)
{
	return a - b;
}

int chengfa(int a, int b)
{
	return a * b;
}

int chufa(int a, int b)
{
	return a / b;
}

int main(void)
{
	char buf[BUFSIZE] = {};
	char *largv[8] = {};
	int largc = 0;
	int left, right;
	int i;

	while (1) {
		printf("[niux@you]￥");
		//"add 1 2"
		i = 0;
		while (1) {
			scanf("%c", buf+i);
			if (buf[i] == '\n') {
				buf[i] = '\0';
				break;
			}
			i++;
		}
		parse(buf, &largc,largv);

		if (!strcmp(largv[0], "quit"))
			break;
		if (largc < 3)
			continue;
		left = atoi(largv[1]);
		right = atoi(largv[2]);
		for (i = 0; i < 4; i++) {
			if (strcmp(largv[0], arr[i].name) == 0) {
				printf("%d %c %d = %d\n", left, arr[i].opch, right, (arr[i].pf)(left, right));	
				break;
			}		
		}
	}

	return 0;	
}

int parse(char *buf, int *argc, char **argv)
{
	*argc = 0;
	while (1) {	
		argv[*argc] = strtok(buf, " \t");
		if (argv[*argc] == NULL)
			break;
		(*argc)++;
		buf = NULL;
	}
}

