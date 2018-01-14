#include <stdio.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>

#define NAMESIZE 32

int main(void)
{
	char name[NAMESIZE] = {};
	char *pw;
	char *aftercrypt;
	struct spwd *sp;

	printf("用户名:");	
	fgets(name, NAMESIZE, stdin);
	name[strlen(name)-1] = '\0';
	pw = getpass("密码:");

	sp = getspnam(name);
	//if error

	aftercrypt = crypt(pw, sp->sp_pwdp);
	//if error

	if (!strcmp(aftercrypt, sp->sp_pwdp))
		printf("鉴定成功！！\n");
	else 
		printf("鉴定失败\n");

	return 0;
}

