/*
字符数组 	 
	数组中存放字符 
字符数组初始化
	char arr[3] = {'a', 'b', 'c'};	 
				  //{97, 98, 99} 
字符串：
	由两个以上的字符组成，必须以'\0'结尾
	"a" ---->'a' '\0'
	'a'
字符串数组的初始化
	char str[4] = "abc";
	char str[4] = {"abc"};
	char str[4] = {'a', 'b', 'c', '\0'};

man string.h
strlen();
strcpy(); strncpy();
strcat(); strncat();
strcmp(); strncmp();
strchr();
strrchr();
strstr();

strtok();
strsep();
strdup();
memcpy();
memmove();
memset();
 */
#include <stdio.h>
#include <string.h>

int main(void)
{
	//字符数组
	char str[] = "hello";		
	char *p = "hello";
//	char str[4] = {'a', 'b', 'c', '\0'};

	printf("hahahhah\n");
	*(str+1) = 'm';
	printf("hihihihihi\n");

	printf("%s\n", p);
	printf("%s\n", str);

	printf("%ld\n", strlen(str));

	return 0;
}







