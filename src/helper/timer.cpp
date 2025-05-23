// This code is based on this blog post and this osdev article:
//
// https://expiredpopsicle.com/articles/2017-04-13-DOS_Timer_Stuff/2017-04-13-DOS_Timer_Stuff.html
// https://wiki.osdev.org/Programmable_Interval_Timer
//
// More info on _dos_getvect:
// https://open-watcom.github.io/open-watcom-v2-wikidocs/clib.html#_dos_getvect
//
#include "timer.h"
#include <stdio.h>
#include <string.h>

#ifdef __DOS__
#include <conio.h>
#include <dos.h>
#else
#include <stubs.h>
#endif
#include <stdlib.h>
#include <stdint.h>
#include <dpmiutil.h>

// This is a different one that gets called from 0x08.
//#define TIMER_INTERRUPT 0x1c

#define TIMER_INTERRUPT 0x08
static timer_func_t g_callback = nullptr;
static uint16_t original_pit_count = 0;
static uint64_t timeValue = 0;
static int32_t nextOldTimer = 0;
static uint32_t timerInitCounter = 0;

#ifdef __DOS__
#pragma aux timer_end_of_interrupt =              \
    "mov al,20H",                               \
    "out 20H,al"

#pragma aux timer_cli =                         \
    "cli"

#pragma aux timer_sti =                         \
    "sti"
static void (__interrupt far *oldDosTimerInterrupt)();

void __interrupt far newCustomTimerInterrupt()
{
    timeValue++;

		if (g_callback != nullptr) {
			g_callback(timeValue);
		}

    nextOldTimer -= 10;
    if(nextOldTimer <= 0) {
        nextOldTimer += 182;
				_chain_intr(oldDosTimerInterrupt);
				return;
    }
    timer_end_of_interrupt();
}

void newCustomTimerInterrupt_end()
{
	/* Function-End marker */
}
#endif

uint64_t timer_get()
{
	#ifdef __DOS__
    return timeValue;
	#else
		return 0;
	#endif
}

uint16_t timer_get_pit_count()
{
	uint16_t count = 0;
	#ifdef __DOS__
	timer_cli();
	outp(0x43, 0x00);
	count = inp(0x40);     // Low byte
	count |= inp(0x40)<<8; // High byte
	timer_sti();
	#endif
	return count;
}

/**
 * Configures the PIT count
 * @see https://expiredpopsicle.com/articles/2017-04-13-DOS_Timer_Stuff/2017-04-13-DOS_Timer_Stuff.html
 */
void timer_set_pit_count(uint16_t c)
{
	#ifdef __DOS__

	timer_cli();
	// There's a ton of options encoded into this one byte I'm going
	// to send to the PIT here so...

	// 0x34 = 0011 0100 in binary.

	// 00  = Select counter 0 (counter divisor)
	// 11  = Command to read/write counter bits (low byte, then high
	//       byte, in sequence).
	// 010 = Mode 2 - rate generator.
	// 0   = Binary counter 16 bits (instead of BCD counter).

	// more info: see the timer.md

	outp(0x43, 0x34);

	// Set divisor low byte.
	outp(0x40, (uint8_t)(c & 0xff));

	// Set divisor high byte.
	outp(0x40, (uint8_t)((c >> 8) & 0xff));
	timer_sti();
	#endif
}

/**
 * Init PIT timer, setting a call frequency and a callback function
 */
void timer_init(timer_func_t callback)
{
	#ifdef __DOS__
		// The clock we're dealing with here runs at 1.193182mhz, so we
		// just divide 1.193182 by the number of triggers we want per
		// second to get our divisor.
		uint32_t c = 1193181 / 1000;

    // Increment ref count and refuse to init if we're already
    // initialized.
    timerInitCounter++;
    if(timerInitCounter > 1) {
			return;
    }

		g_callback = callback;

		original_pit_count = timer_get_pit_count();
		timer_set_pit_count(c);

		if (
			(DPMI_LOCK_FUNC(newCustomTimerInterrupt) != 0) ||
			(DPMI_LOCK_VAR(timeValue) != 0) ||
			(DPMI_LOCK_VAR(nextOldTimer) != 0) ||
			(DPMI_LOCK_VAR(g_callback) != 0)
		) {
			printf("Lock failed\n");
			return;
		}
    // Swap out interrupt handlers.
    oldDosTimerInterrupt = _dos_getvect(TIMER_INTERRUPT);
    _dos_setvect(TIMER_INTERRUPT, newCustomTimerInterrupt);

	#endif
}

void timer_shutdown(void)
{
	#ifdef __DOS__
    // Decrement ref count and refuse to shut down if we're still in
    // use.
    timerInitCounter--;
    if(timerInitCounter > 0) {
        return;
    }

		timer_set_pit_count(original_pit_count);

    // Restore original timer interrupt handler.
    _dos_setvect(TIMER_INTERRUPT, oldDosTimerInterrupt);

		if (
			(DPMI_UNLOCK_FUNC(newCustomTimerInterrupt) != 0) ||
			(DPMI_UNLOCK_VAR(timeValue) != 0) ||
			(DPMI_UNLOCK_VAR(nextOldTimer) != 0) ||
			(DPMI_UNLOCK_VAR(g_callback) != 0)
		) {
			printf("Lock failed\n");
			return;
		}
	#endif
}

void timer_delay(uint64_t ms)
{
    uint64_t startTimer = timer_get();
    while(timer_get() - startTimer < ms) {
    }
}
