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
#include <mouse.h>
#include <keyboard.h>

#define KBD_INTERRUPT 9

#ifdef __DOS__
#pragma aux cpu_hlt = \
	"hlt"
#else
inline void cpu_hlt(void) {}
#endif

bool needs_repaint;
bool g_has_mouse;

void paint(void) {
	uint16_t i, x, y;
	char buf[3];
	bool *keys;

	if (! needs_repaint) {
		return;
	}
	mouse_hide();
	for (i = 0; i < 128; i++) {
		x = (i % 16) * 3;
		y = i / 16;
		snprintf(buf, 3, "%2x", i);
		keys = kbd_get_state()->keys;
		textmode_print(buf, x, y, keys[i] ? 0x1e : 0x17);
	}
	mouse_show();
	needs_repaint = false;
}

void shutdown(void)
{
	mouse_hide();
	if (g_has_mouse) {
		mouse_init();
	}
	kbd_interrupt_shutdown();
	textmode_setmode(3);
}

int main(void) {
	char buf[81];
	bool done = false;
	mouse_callback_data_t *mouse_data;
	uint16_t ch;

	textmode_setmode(3);
	textmode_clear(0x17);
	textmode_print("Press Alt+X to Quit", 0, 24, 0x1a);

	kbd_interrupt_init();
	g_has_mouse = mouse_init();

	atexit(shutdown);

	if (g_has_mouse) {
		if (mouse_set_predefined_eventhandler(EVENT_MOUSE_ALL) != 0) {
			exit(-1);
		}
		mouse_show();
	};

	needs_repaint = true;

	while (!done) {
		paint();
		cpu_hlt();
		if (kbd_get_state()->changed) {
			needs_repaint = true;
			kbd_get_state()->changed = false;
		}
		if (kbhit()) {
			ch = kbd_getkey();
			snprintf(buf, 5, "%4x", ch);
			textmode_print(buf, 5, 23, 0x1e);
			if (ch == 0x2d00) {
				done = true;
			}
		}

		mouse_data = mouse_get_callback_data();
		if (mouse_data->has_event) {
			mouse_data->has_event = false;

			snprintf(buf, 81, "Mouse (%2d): X: %3d Y: %3d State: %3b",
				mouse_data->code,
				mouse_data->x_pos,
				mouse_data->y_pos,
				mouse_data->button_state
			);

			textmode_print(buf, 0, 22, 0x1b);
		}
	}
}
