#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <intrhelp.h>
#include <stdint.h>

int kbd_interrupt_init();
void kbd_interrupt_shutdown();
uint16_t kbd_getkey();
bool *kbd_get_state();

#ifdef __cplusplus
}
#endif
#endif
