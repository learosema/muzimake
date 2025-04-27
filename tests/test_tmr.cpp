#include <stdio.h>
#include <stdlib.h>
#include "textmode.h"
#ifdef __DOS__
#include <dos.h>
#include <conio.h>
#else
#include <stubs.h>
#endif

#include "timer.h"
#include <dpmiutil.h>
static volatile int counter = 0;



static void timer_func(uint64_t time_value)
{
	counter++;

	// we can print via textmode_print (printf won't work)
	textmode_print("Muh", 0, 0, (counter & 0x0f) + 16);
}


int main(int argc, char const *argv[])
{
	char buf[256];
	int ch = 0;
	textmode_setmode(3);
	textmode_clear(0x1f);
	textmode_print("Hit ESC to quit", 0, 24, 0x1a);
	timer_init(timer_func);

	while (ch != 27) {
		if (kbhit()) {
			ch = getch();
		}
		snprintf(buf, 256, "Main program: %d", counter);
		textmode_print(buf, 0, 2, 0x1e);
		timer_delay(10);
	}
	textmode_setmode(3);
	timer_shutdown();
	return 0;
}
