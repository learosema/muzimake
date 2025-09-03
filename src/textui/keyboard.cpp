#include <stdio.h>
#ifdef __DOS__
#include <dos.h>
#include <conio.h>
#ifdef __386__
#include <dpmiutil.h>
#else
#define DPMI_LOCK_FUNC(x) 0
#define DPMU_LOCK_VAR(x) 0
#endif
#else
#include <stubs.h>
#endif
#include <intrhelp.h>
#include <stdint.h>
#include <string.h>

#include "keyboard.h"
#define KBD_INTERRUPT 9
#include "asmstuff.h"

static kbd_state_t g_keystate = {0};
static interrupt_func_t old_keyboard_interrupt = nullptr;


void __interrupt far new_keyboard_interrupt()
{
	#ifdef __DOS__
	asm_cli();
	g_keystate.has_event = true;
	asm_sti();
	_chain_intr(old_keyboard_interrupt);
	#endif
}

void new_keyboard_interrupt_end()
{
	/* Function-End marker */
}

void kbd_read()
{
	g_keystate.last = inp(0x60);
	bool old_state = g_keystate.keys[(g_keystate.last & 0x7f)];
	bool new_state = (g_keystate.last & 0x80) == 0;
	g_keystate.keys[(g_keystate.last & 0x7f)] = new_state;
	g_keystate.changed = old_state != new_state;
}


int kbd_interrupt_init()
{
	#ifdef __DOS__
		if ((DPMI_LOCK_FUNC(new_keyboard_interrupt) != 0) ||
				(DPMI_LOCK_VAR(g_keystate))) {
			fprintf(stderr, "Locks failed\n");
			return -1;
		}
		old_keyboard_interrupt = _dos_getvect(KBD_INTERRUPT);
    _dos_setvect(KBD_INTERRUPT, new_keyboard_interrupt);
	#endif
	return -1;
}

void kbd_interrupt_shutdown()
{
	memset(&g_keystate, 1, sizeof(g_keystate));
	#ifdef __DOS__
	_dos_setvect(KBD_INTERRUPT, old_keyboard_interrupt);
	#endif
}

uint16_t kbd_getkey()
{
	uint16_t ch = getch();
	if (ch == 0) {
		ch = getch() << 8;
	}
	return ch;
}

kbd_state_t *kbd_get_state()
{
	kbd_read();
	return &g_keystate;
}

void kbd_clear_buffer()
{
	#ifdef __DOS__
	#ifdef __I836__
	uint16_t *buffer_head = (uint16_t *)0x41a;
	uint16_t *buffer_tail = (uint16_t *)0x41c;
	*buffer_head = *buffer_tail;
	#endif
	#ifdef __I86__
	uint16_t far *buffer_head = (uint16_t far *)MK_FP(0x0040, 0x001a);
	uint16_t far *buffer_tail = (uint16_t far *)MK_FP(0x0040, 0x001c);
	*buffer_head = *buffer_tail;
	#endif
	#endif
}

/**
 * Like kbd_getkey() but uses the int16 routine rather than getch
 */
uint16_t kbd_getkey_int16()
{
	#ifdef __DOS__
	union REGS regs = {0};
	regs.h.ah = 0;
	#ifdef __386__
	int386(0x16, &regs, &regs);
	#endif
	#ifdef __I86__
	int86(0x16, &regs, &regs);
	#endif
	return regs.h.al == 0 ? regs.w.ax : (regs.w.ax & 0xff);
	#endif
	return 0;
}

/**
 * Get pressed/active states for special keys, as a 8-bit bitmask.
 *
 * 7 Insert active
 * 6 Caps Lock active
 * 5 Num Lock active
 * 4 Scroll Lock active
 * 3 either Alt pressed
 * 2 either Ctrl pressed
 * 1 Left Shift pressed
 * 0 Right Shift pressed
 * @returns uin8_t bitmask
 */
uint8_t kbd_get_shift_flags()
{
	uint8_t result = 0;
	#ifdef __DOS__
	union REGS regs = {0};
	regs.h.ah=2;
	#ifdef __386__
	int386(0x16, &regs, &regs);
	#endif
	#ifdef __I86__
	int86(0x16, &regs, &regs);
	#endif
	result = regs.h.al;
	#endif
	return result;
}
