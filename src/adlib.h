#ifndef __ADLIB_H__
#define __ADLIB_H__

#include <stdint.h>

#define ADLIB_OFFSET 0x388
#define ADLIB_DATA 0x389

#define ADLIB_REG_DRUM 0xBD
#define ADLIB_REG_AR_DR 0x60
#define ADLIB_REG_SL_RR 0x80
#define ADLIB_REG_FNL 0xA0
#define ADLIB_REG_KON_BLOCK_FNH 0xB0

#define ADLIB_BASSDRUM 0x10
#define ADLIB_SNARE 0x08
#define ADLIB_TOMTOM 0x04
#define ADLIB_CYMBAL 0x02
#define ADLIB_HIHAT 0x01

void adlib_reg(uint8_t reg, uint8_t val);
void adlib_reset();
void adlib_rhythm_mode();
void adlib_drum_machine();

#endif
