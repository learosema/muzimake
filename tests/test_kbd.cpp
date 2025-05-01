#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef __DOS__
#include <dos.h>
#include <conio.h>
#else
#include <stubs.h>
#endif

#include <textmode.h>
#include <events.h>
#include <mouse.h>


#define KBD_INTERRUPT 9
#define MOUSE_INTERRUPT 12

#ifdef __DOS__
typedef void (__interrupt __far *interrupt_func_t)();
#define INTERRUPT __interrupt __far
#define FAR __far
#else
typedef void (*interrupt_func_t)();
#define INTERRUPT
#define FAR
#endif
static bool needs_repaint;
static bool g_keys[128];
static uint8_t g_counter;
static interrupt_func_t old_keyboard_interrupt;
static interrupt_func_t old_mouse_interrupt;

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
	needs_repaint = true;

	_chain_intr(old_keyboard_interrupt);
	#endif
}

static void FAR mouse_events() {
	static char buf[4];
	static uint8_t counter = 0;
	g_counter++;
	// there is no snprintf inside interrupts :)
	buf[3] = 0;
	buf[2] = 48 + (g_counter % 10);
	buf[1] = 48 + (g_counter / 10) % 10;
	buf[0] = 48 + (g_counter / 100) % 10;
	textmode_print(buf, 0, 22, 0x1c);
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

void mouse_init_interrupt()
{
	#ifdef __DOS__
		g_counter = 0;
		old_mouse_interrupt = _dos_getvect(MOUSE_INTERRUPT);
		_dos_setvect(MOUSE_INTERRUPT, new_mouse_interrupt);
	#endif
}

void mouse_reset_interrupt()
{
	#ifdef __DOS__
		_dos_setvect(MOUSE_INTERRUPT, old_mouse_interrupt);
	#endif
}

uint16_t kbd_getkey() {
	uint16_t ch = getch();
	if (ch == 0) {
		ch = getch() << 8;
	}
	return ch;
}

void paint() {
	uint16_t i, x, y;
	char buf[3];

	if (! needs_repaint) {
		return;
	}
	mouse_hide();
	for (i = 0; i < 128; i++) {
		x = (i % 16) * 3;
		y = i / 16;
		snprintf(buf, 3, "%2x", i);
		textmode_print(buf, x, y, g_keys[i] ? 0x1e : 0x17);
	}
	mouse_show();
	needs_repaint = false;
}

int main() {

	bool done = false;
	uint16_t ch;

	textmode_setmode(3);
	textmode_clear(0x17);
	textmode_print("Press Alt+X to Quit", 0, 24, 0x1a);
	kbd_init();
	mouse_show();
	mouse_init_interrupt();
	needs_repaint = true;

	while (!done) {
		paint();
		if (kbhit()) {
			ch = kbd_getkey();

			char buf[5];
			snprintf(buf, 5, "%4x", ch);
			textmode_print(buf, 5, 23, 0x1e);
			if (ch == 0x2d00) {
				done = true;
			}
		}
	}

	kbd_shutdown();
	mouse_reset_interrupt();
	mouse_hide();
	textmode_setmode(3);
}
