#include <stdio.h>
/*#开头的预处理命令
 #include 头文件
 头文件中包含的内容：
 				函数的声明、类型的定义
 包含系统头文件<>
 包含自己的头文件""(不是必要的)
*/

/*
	main函数是函数的入口函数,要求有且仅有一个
	main函数几种形式
	1.int main(void)
	2.int main(int argc, char **argv)
	3.int main(int argc, char *argv[])	
 */
int main(void)
{
	printf("hello world\n");
	/*终端(标准输出)输出
		\表示转义字符
	   	\n换行
		\r回车（回到行首）
		\b退格
		\t制表符(Tab)	
	 * */

	return 0;
	/*
	 函数的返回值
	 0表示成功
	 非0表示失败
	 * */
}

