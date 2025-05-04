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

#include <dpmiutil.h>

#define KBD_INTERRUPT 9

#ifdef __DOS__
#pragma aux cpu_hlt = \
	"hlt"
#else
inline void cpu_hlt(void) {}
#endif

static bool needs_repaint;
static bool g_keys[128];
static interrupt_func_t old_keyboard_interrupt = nullptr;

bool g_has_mouse;

/* Data touched at mouse callback time -- they are
   in a structure to simplify calculating the size
   of the region to lock.
*/
struct mouse_callback_data_s {

	bool has_event;

	uint16_t code;

	uint16_t x_pos;
	uint16_t y_pos;

	int16_t x_counts;
	int16_t y_counts;

	uint16_t button_state;

} g_mouse_data = { 0 };

#pragma off( check_stack )
void _loadds far mouse_handler( int max, int mbx, int mcx, int mdx, int msi, int mdi )
{
#pragma aux mouse_handler __parm [__eax] [__ebx] [__ecx] [__edx] [__esi] [__edi]

	g_mouse_data.has_event = true;

	g_mouse_data.code = (uint16_t)max;

	g_mouse_data.button_state = (uint16_t)mbx;

	g_mouse_data.x_pos = (uint16_t)mcx;
  g_mouse_data.y_pos = (uint16_t)mdx;

	g_mouse_data.x_counts = (int16_t)msi;
  g_mouse_data.y_counts = (int16_t)mdi;
}

/* Dummy function so we can calculate size of
  code to lock (mouse_handler_end - mouse_handler).
*/
void mouse_handler_end( void )
{
}
#pragma on(check_stack)

static void INTERRUPT new_keyboard_interrupt() {
	#ifdef __DOS__
	static char buf[4];
	uint8_t kbd = inp(0x60);

	// there is no snprintf inside interrupts :)
	buf[3] = 0;
	buf[2] = 48 + (kbd % 10);
	buf[1] = 48 + (kbd / 10) % 10;
	buf[0] = 48 + (kbd / 100) % 10;
	// textmode_print(buf, 0, 23, 0x1e);

	uint8_t code = kbd & 0x7f;
	bool pressed = (kbd & 0x80) == 0;
	g_keys[code] = pressed;
	needs_repaint = true;

	_chain_intr(old_keyboard_interrupt);
	#endif
}

void kbd_init(void)
{
	#ifdef __DOS__
		old_keyboard_interrupt = _dos_getvect(KBD_INTERRUPT);
    _dos_setvect(KBD_INTERRUPT, new_keyboard_interrupt);
	#endif
}

void kbd_shutdown(void) {
	memset(g_keys, 128, 0);
	#ifdef __DOS__
	_dos_setvect(KBD_INTERRUPT, old_keyboard_interrupt);
	#endif
}

uint16_t kbd_getkey(void) {
	uint16_t ch = getch();
	if (ch == 0) {
		ch = getch() << 8;
	}
	return ch;
}

void paint(void) {
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

int setup_mouse_callback(void)
{
	#ifdef __DOS__
	// All code and touched memory of the mouse callback needs to be locked.
	// Locked means: the DPMI Host must not move this region (which can happen
	// due to swapping, done by a Virtual Memory Manager)

	if (dpmi_lock_linear_region(
			&g_mouse_data,
			sizeof(g_mouse_data)) != 0) {
		return -1;
	}

	if (dpmi_lock_linear_region(
		(void near *)mouse_handler,
		(char *)mouse_handler_end - (char near *)mouse_handler) != 0) {
		return -1;
	}

	mouse_set_eventhandler((far_function_ptr_t)mouse_handler,
		EVENT_MOUSEDOWN_L | EVENT_MOUSEUP_L | EVENT_MOUSEDOWN_R | EVENT_MOUSEUP_R);

	return 0;
	#else
	return -1;
	#endif
}

void shutdown(void)
{
	mouse_hide();
	if (g_has_mouse) {
		mouse_init();
	}
	kbd_shutdown();
	textmode_setmode(3);
}

int main(void) {
	char buf[81];
	bool done = false;
	uint16_t ch;

	textmode_setmode(3);
	textmode_clear(0x17);
	textmode_print("Press Alt+X to Quit", 0, 24, 0x1a);

	kbd_init();
	g_has_mouse = mouse_init();

	atexit(shutdown);

	if (g_has_mouse) {
		if (setup_mouse_callback() != 0) {
			exit(-1);
		}
		mouse_show();
	};

	needs_repaint = true;

	while (!done) {
		paint();
		cpu_hlt();
		if (kbhit()) {
			ch = kbd_getkey();
			snprintf(buf, 5, "%4x", ch);
			textmode_print(buf, 5, 23, 0x1e);
			if (ch == 0x2d00) {
				done = true;
			}
		}

		if (g_mouse_data.has_event) {
			g_mouse_data.has_event = false;

			snprintf(buf, 81, "Mouse (%2d): X: %3d Y: %3d State: %3b",
				g_mouse_data.code,
				g_mouse_data.x_pos,
				g_mouse_data.y_pos,
				g_mouse_data.button_state
			);

			textmode_print(buf, 0, 22, 0x1b);
		}
	}
}
