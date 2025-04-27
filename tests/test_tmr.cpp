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
// https://open-watcom.github.io/open-watcom-v2-wikidocs/clib.html#_dos_getvect

#ifdef __DOS__



void (__interrupt __far *prev_timer_func)();
void __interrupt __far timer_func(void)
{
	counter++;
	// we can print via textmode_print
	textmode_print("Muh", 0, 0, (counter & 0x0f) + 16);
	// can we malloc here?
	void *test= malloc(16);
	free(test);
	_chain_intr(prev_timer_func);
}
#endif

int main(int argc, char const *argv[])
{
	char buf[256];
	int ch = 0;
	textmode_setmode(3);
	textmode_clear(0x1f);
	textmode_print("Hit ESC to quit", 0, 49, 0x1a);
	uint16_t orig_count = timer_get_pit_count();
	timer_set_pit_count(timer_calc_freq(1000));

	#ifdef __DOS__
	// dpmi_lock_linear_region((uint32_t)(&counter), 4);
	prev_timer_func = _dos_getvect(0x8);
	_dos_setvect(0x8, timer_func);
	#endif

	while (ch != 27) {
		if (kbhit()) {
			ch = getch();
		}
		snprintf(buf, 256, "Main program: %d", counter);
		textmode_print(buf, 0, 2, 0x1e);
		delay(1000);
	}
	textmode_setmode(3);

	timer_set_pit_count(orig_count);
	#ifdef __DOS__
	_dos_setvect(0x8, prev_timer_func);
	#endif
	return 0;
}
