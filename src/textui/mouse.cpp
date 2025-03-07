#include <dos.h>

#include "mouse.h"


#ifdef __386__
#define INTR int386
#else
#define INTR int86
#endif


/**
 * Mouse Init and get Installed Flag
 * @returns true, if a mouse driver is installed
 */
bool mouse_init()
{
	#ifdef __DOS__
	union REGS regs;
	regs.w.ax = 0;
	INTR(0x33, &regs, &regs);
	return (regs.w.ax == 0xffff);
	#else
	return false;
	#endif
}

/**
 * Show mouse cursor
 */
void mouse_show()
{
	#ifdef __DOS__
	union REGS regs;
	regs.w.ax = 0x01;
	INTR(0x33, &regs, &regs);
	#endif
}


/**
 * Hide mouse cursor
 */
void mouse_hide()
{
	#ifdef __DOS__
	union REGS regs;
	regs.w.ax = 0x02;
	INTR(0x33, &regs, &regs);
	#endif
}

/**
 * Read mouse status
 */
void mouse_get_status(MOUSE_STATUS *status)
{
	#ifdef __DOS__
	union REGS regs;
	regs.w.ax = 0x03;
	INTR(0x33, &regs, &regs);
	status->mouseX = regs.w.cx;
	status->mouseY = regs.w.dx;
	status->buttons = regs.w.bx;
	#endif
}
