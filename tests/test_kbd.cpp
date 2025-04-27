#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __DOS__
#include <dos.h>
#include <conio.h>
#endif

#include "textmode.h"
#include "events.h"

#ifdef __DOS__
#define KBD_INTERRUPT 9
typedef void (__interrupt __far *interrupt_func_t)();
#define INTRFUNC __interrupt __far
#pragma aux kbd_cli =                         \
    "cli"

#pragma aux kbd_sti =                         \
    "sti"

#pragma aux kbd_end_of_interrupt =              \
    "mov al,20H",                               \
    "out 20H,al"

#else
typedef void (*interrupt_func_t)();
#define INTERRUPT_FUNC
#endif
static char g_keys[128];

static interrupt_func_t old_keyboard_interrupt;
static void INTERRUPT_FUNC new_keyboard_interrupt() {
	#ifdef __DOS__
	// TODO FIGURE OUT HOW THIS WORKS
	int ch = inp(0x61);
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
	#ifdef __DOS__
	_dos_setvect(KBD_INTERRUPT, old_keyboard_interrupt);
	#endif
}


int main() {
	memset(g_keys, 128, 0);
}
