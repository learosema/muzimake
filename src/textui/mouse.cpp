#ifdef __DOS__
#include <dos.h>
#else
#include <intstubs.h>
#endif

#include <intrhelp.h>
#include <stubs.h>

#if defined __DOS__ && defined __386__
#include <dpmiutil.h>
#define INTR int386
#define INTRX int386x
#elif defined __DOS__ && defined __I86__
#define DPMI_LOCK_FUNC(x) 0
#define DPMI_LOCK_VAR(x)  0
#define INTR int86
#define INTRX int86x
#else
#define DPMI_LOCK_FUNC(x) 0
#define DPMI_LOCK_VAR(x)  0
#define INTR
#define INTRX
#endif

#include "mouse.h"

// https://github.com/open-watcom/open-watcom-v2/blob/master/docs/doc/rsi/dos4gwqa.gml
#ifdef __DOS__
#ifdef __386__
#else
#endif
#endif

/* Data touched at mouse callback time -- they are
   in a structure to simplify calculating the size
   of the region to lock.
*/
mouse_callback_data_t g_mouse_data = { 0 };

/**
 * Mouse Init and get Installed Flag.
 * Also resets the callback.
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

int mouse_set_eventhandler(far_function_ptr_t handler, uint8_t call_mask)
{
	// https://stanislavs.org/helppc/int_33-c.html
	#ifdef __DOS__
	union REGS regs;
	union REGS oregs;
	struct SREGS sregs;
	segread( &sregs );

	regs.w.ax = 0x0C;
	regs.w.cx = call_mask;
	sregs.es = FP_SEG(handler);
	regs.x.edx = FP_OFF(handler);
	INTRX(0x33, &regs, &oregs, &sregs);
	return 0;
	#endif
}

#pragma off ( check_stack )
void _loadds far mouse_handler( int max, int mbx, int mcx, int mdx, int msi, int mdi )
{
#pragma aux mouse_handler __parm [__eax] [__ebx] [__ecx] [__edx] [__esi] [__edi]

	if (g_mouse_data.has_event && (max & 1)) {
		// dismiss mouse move event if there is an unprocessed button event.
		// TODO: have separate event structures for each mouse event type
		//       (so neither clicks nor moves get lost)
		return;
	}

	g_mouse_data.has_event = true;

	g_mouse_data.code = (uint16_t)max;

	g_mouse_data.button_state = (uint16_t)mbx;

	g_mouse_data.x_pos = (uint16_t)mcx;
  g_mouse_data.y_pos = (uint16_t)mdx;

	g_mouse_data.x_counts = (int16_t)msi;
  g_mouse_data.y_counts = (int16_t)mdi;
}

void mouse_handler_end( void )
{
	/* Function-End marker */
}

/**
 * Installs predefined a callback that fills the g_mouse_data structure.
 */
int mouse_set_predefined_eventhandler(uint8_t call_mask)
{
	#ifdef __DOS__
	// All code and touched memory of the mouse callback needs to be locked.
	// Locked means: the DPMI Host must not move this region (which can happen
	// due to swapping, done by a Virtual Memory Manager)

	if (DPMI_LOCK_VAR(g_mouse_data) != 0) {
		return -1;
	}

	if (DPMI_LOCK_FUNC(mouse_handler) != 0) {
		return -1;
	}

	mouse_set_eventhandler((far_function_ptr_t)mouse_handler, call_mask == 0xff ?
		EVENT_MOUSE_ALL : call_mask);

	return 0;
	#else
	return -1;
	#endif
}

#pragma on ( check_stack )

mouse_callback_data_t *mouse_get_callback_data()
{
	return &g_mouse_data;
}
