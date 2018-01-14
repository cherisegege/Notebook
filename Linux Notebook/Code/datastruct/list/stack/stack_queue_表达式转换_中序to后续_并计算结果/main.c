#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "sequeue.h"

stack_t *stack, *stack1;
queue_t *queue;


int is_number(char ch);
int is_yunsuanfu(char ch);
int prior(char ch);
int dopop_yun(char ch);
int is_kuohao_zuo(char ch);
int dopop_kuo(void);
int move(stack_t *s1, queue_t *s2);
int caculate();

int main(void)
{
//	char da[] = "1*(2+3)-4/5+6";
//	char da[] = "6+5/4-)3+2(*1";
//	+-*1+23/456

//	char da[] = "1+2*3/(4-5)*6+7";
//   char da[] = "7+6*)5-4(/3*2+1";
//  ++1*/*23-4567
	char da[] = "1*(2+3)-4/5+6";
//  123+*45/-6+
	int i = 0;
	char temp;

	stack = stack_init(sizeof(char));
	queue = queue_init(sizeof(char));
	stack1 = stack_init(sizeof(int));

	while(da[i]){
		if(!is_number(da[i])){
			queue_enq(queue,da+i);
		}else if(!is_yunsuanfu(da[i])){
			if(!stack_empty(stack)){
				stack_push(stack, da+i);
			}else if(!prior(da[i])){
				stack_push(stack, da+i);
			}else{
				dopop_yun(da[i]);
			}
		}else if(!is_kuohao_zuo(da[i])){
			stack_push(stack, da+i);		
		}else{
			dopop_kuo();//这时就是检测到了左括号 '('
		}
		i++;
	}

	move(stack, queue);

	caculate();
/*
	while(queue_empty(queue)){
		queue_deq(queue, &temp);
		printf("%c",temp);
	}
	printf("\n");
*/
	stack_destroy(stack);
	queue_destroy(queue);

	return 0;
}

int is_number(char ch)
{
	if(ch >= '0' && ch <= '9'){
		return 0;
	}
	return 1;
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

	if(ch == '+' || ch == '-'){
		if(stack_empty(stack)){
			stack_pop(stack, &temp);
			stack_push(stack, &temp);
			if(temp == '(')
				return 0;
		}
	}
	if(ch == '*' || ch == '/'){
		if(stack_empty(stack)){
			stack_pop(stack, &temp);
			stack_push(stack, &temp);
			if(temp == '+' || temp == '-' || temp == '('){
				return 0;
			}
		}
	}

	return 1;	
}

int dopop_yun(char ch)
{
	char temp;
	int i = 0;
//栈非空，且符号优先级低 
	while(stack_empty(stack)){
		stack_pop(stack, &temp);
		queue_enq(queue, &temp);							
		if(!prior(ch)){
			stack_push(stack, &ch);
			return 0;
		}
	}
	stack_push(stack, &ch);

	return 0;	
}

int is_kuohao_zuo(char ch)
{
	if(ch == '('){
		return 0;
	}
	return 1;
}

int dopop_kuo(void)
{
	char temp;

	while(stack_empty(stack)){
		stack_pop(stack, &temp);
		if(is_kuohao_zuo(temp)){
			queue_enq(queue, &temp);			
		}else{	
			break;
		}
	}
	return 0;
}
int move(stack_t *s1, queue_t *s2)
{
	char temp;

	while(stack_empty(stack)){
		stack_pop(stack, &temp);
		queue_enq(queue, &temp);
	}

	return 0;
}

int _caculate(char ch, int num1, int num2)
{
	int sum;

	switch(ch){
		case '+':
			sum = num1 + num2;
			break;
		case '-':
			sum = num1 - num2;
			break;
		case '*':
			sum = num1 * num2;
			break;
		case '/':
			sum = num1 / num2;
			break;	
		default:
			break;
	}

	return sum;
}

int caculate()
{
	char temp;
	int sum = 0, num1, num2;
	int num;

	while(queue_empty(queue)){
		queue_deq(queue, &temp);
		if(!is_number(temp)){
			num = atoi(&temp);
			stack_push(stack1, &num);
		}else if(!is_yunsuanfu(temp)){
			stack_pop(stack1, &num2);
			stack_pop(stack1, &num1);
			sum = _caculate(temp, num1, num2);
			stack_push(stack1, &sum);
		}
	}
	stack_pop(stack1, &sum);
	if(!stack_empty(stack1)){
		printf("the result is %d\n", sum);
	}

	return 0;
}


