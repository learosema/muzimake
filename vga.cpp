#include <conio.h>
/*

TODO: turbo c++ seems to use dos.h, mem.h or something,
but WATCOM uses conio.h for inp
#include <dos.h>
*/
#include "vga.h"

void vga_wait_for_retrace()
{
	while( inp( INPUT_STATUS ) & VRETRACE );
	while( ! (inp( INPUT_STATUS ) & VRETRACE) );
}
