// https://expiredpopsicle.com/articles/2017-04-13-DOS_Timer_Stuff/2017-04-13-DOS_Timer_Stuff.html
#include "timer.h"
#include <stdio.h>
#include <string.h>

#ifdef __DOS__
#include <conio.h>
#include <dos.h>
#endif
#include <stdlib.h>
#include <stdint.h>

// This is a different one that gets called from 0x08.
//#define TIMER_INTERRUPT 0x1c

#define TIMER_INTERRUPT 0x08

#ifdef __DOS__
#pragma aux timer_endOfInterrupt =              \
    "mov al,20H",                               \
    "out 20H,al"

#pragma aux timer_cli =                         \
    "cli"

#pragma aux timer_sti =                         \
    "sti"


static uint32_t timeValue = 0;
static int32_t nextOldTimer = 0;
static uint32_t timerInitCounter = 0;

static void (__interrupt __far *oldDosTimerInterrupt)();

static void __interrupt __far newCustomTimerInterrupt()
{
    timeValue++;
		printf("%d", timeValue);
    nextOldTimer -= 10;
    if(nextOldTimer <= 0) {

        nextOldTimer += 182;
        oldDosTimerInterrupt();

    } else {

        // Make sure we still execute the "HEY I'M DONE WITH THIS
        // INTERRUPT" signal.

        timer_endOfInterrupt();
    }
}
#endif

uint32_t timer_get()
{
	#ifdef __DOS__
    return timeValue;
	#else
		return 0;
	#endif
}

void timer_init()
{
	#ifdef __DOS__
    // The clock we're dealing with here runs at 1.193182mhz, so we
    // just divide 1.193182 by the number of triggers we want per
    // second to get our divisor.
    uint32_t c = 1193181 / (uint32_t)1000;

    // Increment ref count and refuse to init if we're already
    // initialized.
    timerInitCounter++;
    if(timerInitCounter > 1) {
        return;
    }

    // Swap out interrupt handlers.
    oldDosTimerInterrupt = _dos_getvect(TIMER_INTERRUPT);
    _dos_setvect(TIMER_INTERRUPT, newCustomTimerInterrupt);

    timer_cli();

    // There's a ton of options encoded into this one byte I'm going
    // to send to the PIT here so...

    // 0x34 = 0011 0100 in binary.

    // 00  = Select counter 0 (counter divisor)
    // 11  = Command to read/write counter bits (low byte, then high
    //       byte, in sequence).
    // 010 = Mode 2 - rate generator.
    // 0   = Binary counter 16 bits (instead of BCD counter).

    outp(0x43, 0x34);

    // Set divisor low byte.
    outp(0x40, (uint8_t)(c & 0xff));

    // Set divisor high byte.
    outp(0x40, (uint8_t)((c >> 8) & 0xff));

    timer_sti();
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

    timer_cli();

    // Send the same command we sent in timer_init() just so we can
    // set the timer divisor back.
    outp(0x43, 0x34);

    // FIXME: I guess giving zero here resets it? Not sure about this.
    // Maybe we should save the timer values first.
    outp(0x40, 0);
    outp(0x40, 0);

    timer_sti();

    // Restore original timer interrupt handler.
    _dos_setvect(TIMER_INTERRUPT, oldDosTimerInterrupt);
	#endif
}

void timer_delay(uint32_t ms)
{
    uint32_t startTimer = timer_get();
    while(timer_get() - startTimer < ms) {
    }
}
