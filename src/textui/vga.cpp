#ifdef __DOS__
#include <conio.h>
#include <dos.h>
#else
#include <stubs.h>
#endif

#include "vga.h"

void vga_wait_for_retrace()
{
	#ifdef __DOS__
	while( inp( INPUT_STATUS ) & VRETRACE );
	while( ! (inp( INPUT_STATUS ) & VRETRACE) );
	#endif
}
