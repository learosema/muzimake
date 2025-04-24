#include <stdio.h>
#include <stdlib.h>
#ifdef __DOS__
#include <conio.h>
#include <dos.h>
#else
#include <stubs.h>
#endif

#include "timer.h"

int main(int argc, char const *argv[])
{
	int ch = 0;
	timer_init();
	atexit(timer_shutdown);
	while (ch != 27) {
		printf("Main program");
		delay(1000);
		if (kbhit()) {
			ch = getch();
		}
	}
	return 0;
}
