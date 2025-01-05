// shamelessly taken from codeberg.org/root42/dosadlib,
// adapted for openwatcom v2
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <conio.h>
#include <dos.h>

#include "adlib.h"
#include "vga.h"

static char adlib_reg_drum = 0x00;
static int adlib_osc_off[] =
		{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x08, 0x09, 0x0a,
		 0x0b, 0x0c, 0x0d, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15};


// 0x122 = 290. The 290 is determined by the formula:
//
// F-Num = Music Frequency * 2^(20-Block) / 49716 Hz
// Tone A (440 Hz) is in Block(=Octave) 5
static int adlib_osc_freq[] =
		{0x122, 0x122, 0x122, 0x122, 0x122, 0x122, 0x122, 0x122, 0x122};

void adlib_reg(uint8_t reg, uint8_t val)
{
	int i;
	outp(ADLIB_OFFSET, reg);
	for (i = 0; i < 6; ++i)
		inp(ADLIB_OFFSET);
	outp(ADLIB_DATA, val);
	for (i = 0; i < 35; ++i)
		inp(ADLIB_DATA);
}

void adlib_reset()
{
	int i;
	const char block = 3;        /* frequency block 3 */
	const unsigned int A5 = 290; /* eq 440Hz in Block 3 */

	/* set Wave Select Enable (WSE) bit */
	adlib_reg(0x01, 0x20);
	/* disable speech mode, disable split */
	adlib_reg(0x08, 0x00);
	/* rhythm mode */
	adlib_reg(0xbd, 0x20);
	/* configure ADSR for all oscillators to a medium curve */
	for (i = 0; i < 18; ++i)
	{
		adlib_reg(ADLIB_REG_AR_DR + adlib_osc_off[i], 0x99);
		adlib_reg(ADLIB_REG_SL_RR + adlib_osc_off[i], 0x66);
	}
	/* set all channels to A5 */
	for (i = 0; i < 9; ++i)
	{
		adlib_reg(ADLIB_REG_FNL + i, A5 & 0xFF);
		adlib_reg(ADLIB_REG_KON_BLOCK_FNH + i, (block << 2) | (A5 >> 8));
	}
}

void adlib_rhythm_mode()
{
	int A4 = 70, A7 = 870, block = 3;
	/* enable R bit */
	adlib_reg_drum |= 0x20;
	adlib_reg(ADLIB_REG_DRUM, adlib_reg_drum);
	/* set tom tom to a higher note for better sound */
	adlib_reg(ADLIB_REG_FNL + 8, A7 & 0xFF);
	adlib_reg(ADLIB_REG_KON_BLOCK_FNH + 8, (block << 2) | (A7 >> 8));
	/* set bassdrum to a lower note for better sound */
	adlib_reg(ADLIB_REG_FNL + 6, A4 & 0xFF);
	adlib_reg(ADLIB_REG_KON_BLOCK_FNH + 6, (block << 2) | (A4 >> 8));
	/* set ADSR for CY and HH to something slower */
	adlib_reg(ADLIB_REG_AR_DR + adlib_osc_off[13], 0x88);
	adlib_reg(ADLIB_REG_SL_RR + adlib_osc_off[13], 0x55);
	adlib_reg(ADLIB_REG_AR_DR + adlib_osc_off[17], 0x88);
	adlib_reg(ADLIB_REG_SL_RR + adlib_osc_off[17], 0x55);
}

void adlib_play_drums(char drums)
{
	adlib_reg(ADLIB_REG_DRUM, adlib_reg_drum);
	adlib_reg(ADLIB_REG_DRUM, adlib_reg_drum | drums);
}

#define ticks 512

void adlib_drum_machine()
{
	char kc = 0, drums[ticks];
	unsigned int t = 0, incr = 0;
	memset(drums, 0, ticks);
	adlib_reset();
	adlib_rhythm_mode();

	while (kc != 0x1b)
	{
		if (kbhit())
		{
			if (!incr)
				incr = 1;
			kc = getch();
			switch (kc)
			{
			case '1':
				drums[t % ticks] |= ADLIB_BASSDRUM;
				break;
			case '2':
				drums[t % ticks] |= ADLIB_SNARE;
				break;
			case '3':
				drums[t % ticks] |= ADLIB_TOMTOM;
				break;
			case '4':
				drums[t % ticks] |= ADLIB_CYMBAL;
				break;
			case '5':
				drums[t % ticks] |= ADLIB_HIHAT;
				break;
			default:
				drums[t % ticks] = 0;
				break;
			}
		}
		adlib_play_drums(drums[t % ticks]);
		vga_wait_for_retrace();
		t += incr;
		printf("\b\b\b\b\b\b%5d", (t % ticks) / 32);
	}
}
