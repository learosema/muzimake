#include <stdio.h>
#include <stdlib.h>
#ifdef __DOS__
#include <dos.h>
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

	_chain_intr(prev_timer_func);
}
#endif

int main(int argc, char const *argv[])
{
	int ch = 0;

	#ifdef __DOS__
	// dpmi_lock_linear_region((uint32_t)(&counter), 4);
	prev_timer_func = _dos_getvect(0x8);
	_dos_setvect(0x8, timer_func);
	#endif

	while (1) {
		printf("Main program %d\n", counter);
		delay(1000);
	}
	#ifdef __DOS__
	_dos_setvect(0x8, prev_timer_func);
	#endif
	return 0;
}
