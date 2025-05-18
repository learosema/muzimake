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

static kbd_state_t g_keystate = {0};
static interrupt_func_t old_keyboard_interrupt = nullptr;

void __interrupt far new_keyboard_interrupt()
{
	#ifdef __DOS__
	static char buf[4];
	uint8_t kbd = inp(0x60);

	uint8_t code = kbd & 0x7f;
	bool pressed = (kbd & 0x80) == 0;
	if (pressed != g_keystate.keys[code]) {
		g_keystate.keys[code] = pressed;
		g_keystate.changed = true;
	}

	_chain_intr(old_keyboard_interrupt);
	#endif
}

void new_keyboard_interrupt_end()
{
	/* Function-End marker */
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
	return &g_keystate;
}

void kbd_clear_buffer() {
	/** ' clear keyboard buffer
DEF SEG = &H40
POKE &H1A, PEEK(&H1C) */
	uint16_t *buffer_head = (uint16_t *)0x41a;
	uint16_t *buffer_tail = (uint16_t *)0x41c;
	*buffer_head = *buffer_tail;

}
