#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef __DOS__
#include <dos.h>
#include <conio.h>
#else
#include "stubs.h"
#endif

#include "textmode.h"
#include "events.h"

#ifdef __DOS__
#define KBD_INTERRUPT 9
typedef void (__interrupt __far *interrupt_func_t)();
#define INTERRUPT __interrupt __far
#pragma aux kbd_cli =                         \
    "cli"

#pragma aux kbd_sti =                         \
    "sti"

#pragma aux kbd_end_of_interrupt =              \
    "mov al,20H",                               \
    "out 20H,al"

#else
typedef void (*interrupt_func_t)();
#define INTERRUPT
#endif
static bool g_keys[128];
static interrupt_func_t old_keyboard_interrupt;

static void INTERRUPT new_keyboard_interrupt() {
	#ifdef __DOS__
	static char buf[4];
	uint8_t kbd = inp(0x60);

	// there is no snprintf inside interrupts :)
	buf[3] = 0;
	buf[2] = 48 + (kbd % 10);
	buf[1] = 48 + (kbd / 10) % 10;
	buf[0] = 48 + (kbd / 100) % 10;
	textmode_print(buf, 0, 23, 0x1e);

	uint8_t code = kbd & 0x7f;
	bool pressed = (kbd & 0x80) == 0;
	g_keys[code] = pressed;

	_chain_intr(old_keyboard_interrupt);
	#endif
}

void kbd_init()
{
	#ifdef __DOS__
		old_keyboard_interrupt = _dos_getvect(KBD_INTERRUPT);
    _dos_setvect(KBD_INTERRUPT, new_keyboard_interrupt);
	#endif
}

void kbd_shutdown() {
	memset(g_keys, 128, 0);
	#ifdef __DOS__
	_dos_setvect(KBD_INTERRUPT, old_keyboard_interrupt);
	#endif
}


int main() {
	bool done = false;
	uint16_t ch;
	uint16_t i, x, y;
	char buf[3];
	textmode_setmode(3);
	textmode_clear(0x17);
	textmode_print("Press Alt+X to Quit", 0, 24, 0x1a);
	kbd_init();

	while (!done) {
		for (i = 0; i < 128; i++) {
			x = (i % 16) * 3;
			y = i / 16;
			snprintf(buf, 3, "%2x", i);
			textmode_print(buf, x, y, g_keys[i] ? 0x1e : 0x17);
		}
		if (kbhit()) {
			ch = getch();
			if (ch == 0) {
				ch = getch() << 8;
			}
			char buf[5];
			snprintf(buf, 5, "%4x", ch);
			textmode_print(buf, 5, 23, 0x1e);
			if (ch == 0x2d00) {
				done = true;
			}
		}
	}

	kbd_shutdown();
	textmode_setmode(3);
}
