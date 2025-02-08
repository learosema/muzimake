#ifdef __DOS__
#include <dos.h>
#else
#include <intstubs.h>
#endif

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
 * Set vertical mouse range
 * @param minY minimum row
 * @param maxY maximum row
 */
void mouse_set_vertical_range(uint16_t minY, uint16_t maxY)
{
	union REGS regs;
	regs.w.ax = 8;
	regs.w.cx = minY;
	regs.w.dx = maxY;
	INTR(0x33, &regs, &regs);
}

/**
 * Set horizontal mouse range
 * @param minX minimum col
 *
 */
void mouse_set_horizontal_range(uint16_t minX, uint16_t maxX)
{
	union REGS regs;
	regs.w.ax = 7;
	regs.w.cx = minX;
	regs.w.dx = maxX;
	INTR(0x33, &regs, &regs);
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
