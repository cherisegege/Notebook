#include <stdio.h>

#include "stack.h"

	struct stack_st *stack1, *stack2;


int is_number(char ch);
int is_yunsuanfu(char ch);
int prior(char ch);
int dopop_yun(char ch);
int is_kuohao_you(char ch);
int dopop_kuo(void);
int move(struct stack_st *s1, struct stack_st *s2);


int main(void)
{

//	char da[] = "1*(2+3)-4/5+6";
//	char da[] = "6+5/4-)3+2(*1";
//	+-*1+23/456

//	char da[] = "1+2*3/(4-5)*6+7";
    char da[] = "7+6*)5-4(/3*2+1";
//  ++1*/*23-4567

//char da[] = "1+2/3";
//char da[] = "3/2+1";
//  +1/23
	

	int i = 0;
	char temp;

	stack1 = init_stack(sizeof(char));
	stack2 = init_stack(sizeof(char));

	while(da[i]){
		if(!is_number(da[i])){
			push(stack1,da+i);
		}else if(!is_yunsuanfu(da[i])){
			if(!empty(stack2)){
				push(stack2, da+i);
			}else if(!prior(da[i])){
				push(stack2, da+i);	
			}else{
				dopop_yun(da[i]);
			}
		}else if(!is_kuohao_you(da[i])){
			push(stack2, da+i);
		}else{
			dopop_kuo();//这时就是检测到了左括号 '('
		}
		i++;
	}
	move(stack1, stack2);

	while(empty(stack1)){
		pop(stack1, &temp);
		printf("%c ",temp);
	}
	printf("\n");

	destroy(stack1);
	destroy(stack2);

	return 0;
}

int is_number(char ch)
{
	if(!(ch >= '0' && ch <= '9')){
		return 1;
	}
	return 0;
}

int is_yunsuanfu(char ch)
{
	if(ch == '+' || ch == '-' || ch == '*' || ch == '/'){
		return 0;
	}
	return 1;
}

int prior(char ch)
{
	char temp;

	if(ch == '*' || ch == '/'){
		return 0;
	}else{
		if(empty(stack2)){
			pop(stack2, &temp);
			push(stack2, &temp);
			if(temp == '+' || temp == '-' || temp == ')'){
				return 0;
			}else{
				return 1;
			}
		}
	}
}

int dopop_yun(char ch)
{
	char temp;

	while(1){
		if(!prior(ch)){
			push(stack2, &ch);
			break;
		}else{
			if(empty(stack2)){
				pop(stack2, &temp);
				push(stack1, &temp);
			}else{
				push(stack2, &temp);
				return 0;
			}
		}
	}	
}

int is_kuohao_you(char ch)
{
	if(ch == ')'){
		return 0;
	}
	return 1;
}

int dopop_kuo(void)
{
	char temp;

	while(1){
		if(empty(stack2)){
			pop(stack2, &temp);
			if(is_kuohao_you(temp)){
				push(stack1, &temp);			
			}else{			
				return 0;
			}
		}
	}
}
int move(struct stack_st *s1, struct stack_st *s2)
{
	char temp;

	while(1){
		if(empty(stack2)){
			pop(stack2, &temp);
			push(stack1, &temp);
		}else{
			return 0;
		}
	}
}





