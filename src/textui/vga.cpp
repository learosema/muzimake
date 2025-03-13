#ifdef __DOS__
#include <conio.h>
#include <dos.h>
#else
#include <stubs.h>
#endif

/*

TODO: turbo c++ seems to use dos.h, mem.h or something,
but WATCOM uses conio.h for inp
#include <dos.h>
*/
#include "vga.h"

void vga_wait_for_retrace()
{
	#ifdef __DOS__
	while( inp( INPUT_STATUS ) & VRETRACE );
	while( ! (inp( INPUT_STATUS ) & VRETRACE) );
	#endif
}
