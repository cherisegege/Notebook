#include <stdio.h>
#include <stdlib.h>

#include "alarm.h"

int main(void)
{
	int a, b, c;

	a = alarmany_init(1, "hello", sizeof("hello"));	
	b = alarmany_init(5, "boys", sizeof("boys"));	
	c = alarmany_init(10, "girls", sizeof("girls"));	

	while(1)
		;

	exit(EXIT_SUCCESS);
}


