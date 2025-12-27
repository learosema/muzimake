#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <intrhelp.h>
#include <stdint.h>

typedef struct kbd_state_s {
	bool has_event;
	bool changed;
	uint8_t last;
	bool keys[128];
} kbd_state_t;

int kbd_interrupt_init();
void kbd_interrupt_shutdown();
kbd_state_t *kbd_get_state();
void kbd_clear_buffer();
uint16_t kbd_getkey_int16();
uint16_t kbd_getkey();
uint8_t kbd_get_shift_flags();
void kbd_read();

#ifdef __cplusplus
}
#endif
#endif
