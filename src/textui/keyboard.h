#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <intrhelp.h>
#include <stdint.h>

typedef struct kbd_state_s {
	bool changed;
	bool keys[128];
} kbd_state_t;

int kbd_interrupt_init();
void kbd_interrupt_shutdown();
uint16_t kbd_getkey();
kbd_state_t *kbd_get_state();
void kbd_clear_buffer();

#ifdef __cplusplus
}
#endif
#endif
