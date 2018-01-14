#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>

/*VT 控制码*/

static int glob_x = 3, glob_y = 20;

static int arr[4][4] = {
	{0,1,0,0},
	{0,1,1,0},
	{0,0,1,0},
	{0,0,0,0}
};

static void draw(void)
{
	int i, j;

	printf("\33[2J");

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (arr[i][j]) {
				printf("\33[42m\33[%d;%dH[]", glob_x+i, glob_y+2*j);	
				printf("\33[0m");
			} 
		}
		printf("\n");
	}
}

static void sig_handler(int s)
{
	glob_x++;
	draw();
}

int main(void)
{
	int c;
	struct termios tcold, tcnew;
	struct itimerval tval;
	struct sigaction act;

	tcgetattr(0, &tcold);
	tcnew = tcold;
	tcnew.c_lflag = ~(ECHO | ICANON);  	
	tcsetattr(0, TCSANOW, &tcnew);

	draw();

	//signal(SIGALRM, sig_handler);
	act.sa_handler = sig_handler;
	act.sa_flags = 0;
	sigfillset(&act.sa_mask);
	sigdelset(&act.sa_mask, SIGALRM);

	sigaction(SIGALRM, &act, NULL);

	tval.it_interval.tv_sec = 0;
	tval.it_interval.tv_usec = 500000;
	tval.it_value.tv_sec = 3;
	tval.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &tval, NULL);

	while (1) {
		c = getchar();

		if (c == 'a') {
			glob_y -= 2;
			draw();
		} else if (c == 'd') {
			glob_y += 2;
			draw();
		} else if (c == 's') {
			glob_x += 2;	
			draw();
		}
	}

	tcsetattr(0, TCSANOW, &tcold);

	return 0;
}

