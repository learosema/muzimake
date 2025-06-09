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



void timer_func(uint64_t time_value)
{
	counter++;
}

void timer_func_end() {
	/* Function-End Marker */
}


int main(int argc, char const *argv[])
{
	char buf[256];
	int ch = 0;
	int lock = 0;
	lock += DPMI_LOCK_FUNC(timer_func);
	lock += DPMI_LOCK_VAR(counter);

	if (lock != 0) {
		printf("Locks failed.\n");
		exit(-1);
	}

	textmode_setmode(3);
	textmode_clear(0x1f);
	textmode_print("Hit ESC to quit", 0, 24, 0x1a);

	timer_init(timer_func);

	while (ch != 27) {
		if (kbhit()) {
			ch = getch();
		}
		snprintf(buf, 256, "Counter: %d", counter);
		textmode_print(buf, 0, 2, 0x10 + counter & 0xf);
		timer_delay(10);
	}
	textmode_setmode(3);
	timer_shutdown();
	return 0;
}
