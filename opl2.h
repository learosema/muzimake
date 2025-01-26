/**
 * OPL2 Audio Lib for Arduino & Pi
 *
 * YM3812 OPL2 Audio Library for Arduino, Raspberry Pi and Orange Pi v2.1.0
 * Code by Maarten Janssen (maarten@cheerful.nl) 2016-12-18
 * WWW.CHEERFUL.NL
 *
 * Modified from class- to function-style by SuperIlu:
 * https://github.com/SuperIlu/lib16/
 *
 * Moved types into "instr.h", extended instruments to also store the drum type
 * https://github.com/learosema/muzimake/
 *
 * Last updated 2025-01-26
 *
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 * Details about the YM3812 and YMF262 chips can be found at http://www.shikadi.net/moddingwiki/OPL_chip
 *
 * This library is open source and provided as is under the MIT software license, a copy of which is provided as part of
 * the project's repository. This library makes use of Gordon Henderson's Wiring Pi.
 * WWW.CHEERFUL.NL
 */
#ifndef __OPL2_H_
#define __OPL2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "instr.h"

/* ======================================================================
** prototypes
** ====================================================================== */
bool opl2_getDeepTremolo();
bool opl2_getDeepVibrato();
bool opl2_getEnvelopeScaling(uint8_t channel, uint8_t operatorNum);
bool opl2_getKeyOn(uint8_t channel);
bool opl2_getMaintainSustain(uint8_t channel, uint8_t operatorNum);
bool opl2_getNoteSelect();
bool opl2_getPercussion();
bool opl2_getTremolo(uint8_t channel, uint8_t operatorNum);
bool opl2_getVibrato(uint8_t channel, uint8_t operatorNum);
bool opl2_getWaveFormSelect();
bool opl2_init();
float opl2_getFrequency(uint8_t channel);
float opl2_getFrequencyStep(uint8_t channel);
uint16_t opl2_getFNumber(uint8_t channel);
uint16_t opl2_getFrequencyFNumber(uint8_t channel, float frequency);
uint16_t opl2_getNoteFNumber(uint8_t note);
uint8_t opl2_getAttack(uint8_t channel, uint8_t operatorNum);
uint8_t opl2_getBlock(uint8_t channel);
uint8_t opl2_getChannelVolume(uint8_t channel);
uint8_t opl2_getDecay(uint8_t channel, uint8_t operatorNum);
uint8_t opl2_getDrums();
uint8_t opl2_getFeedback(uint8_t channel);
uint8_t opl2_getFrequencyBlock(float frequency);
uint8_t opl2_getMultiplier(uint8_t channel, uint8_t operatorNum);
uint8_t opl2_getRelease(uint8_t channel, uint8_t operatorNum);
uint8_t opl2_getScalingLevel(uint8_t channel, uint8_t operatorNum);
uint8_t opl2_getSustain(uint8_t channel, uint8_t operatorNum);
uint8_t opl2_getSynthMode(uint8_t channel);
uint8_t opl2_getVolume(uint8_t channel, uint8_t operatorNum);
uint8_t opl2_getWaveForm(uint8_t channel, uint8_t operatorNum);
void opl2_createInstrument(instrument_t *instrument);
void opl2_getInstrument(uint8_t channel, instrument_t *instrument);
void opl2_loadInstrument(const unsigned char *data, instrument_t *instrument);
void opl2_playDrum(uint8_t drum, uint8_t octave, uint8_t note);
void opl2_playNote(uint8_t channel, uint8_t octave, uint8_t note);
void opl2_reset();
void opl2_setAttack(uint8_t channel, uint8_t operatorNum, uint8_t attack);
void opl2_setBlock(uint8_t channel, uint8_t block);
void opl2_setChannelVolume(uint8_t channel, uint8_t volume);
void opl2_setDecay(uint8_t channel, uint8_t operatorNum, uint8_t decay);
void opl2_setDeepTremolo(bool enable);
void opl2_setDeepVibrato(bool enable);
void opl2_setDrumInstrument(instrument_t *instrument, uint8_t drumType, float volume);
void opl2_setDrums(bool bass, bool snare, bool tom, bool cymbal, bool hihat);
void opl2_setDrumsByte(uint8_t drums);
void opl2_setEnvelopeScaling(uint8_t channel, uint8_t operatorNum, bool enable);
void opl2_setFeedback(uint8_t channel, uint8_t feedback);
void opl2_setFNumber(uint8_t channel, uint16_t fNumber);
void opl2_setFrequency(uint8_t channel, float frequency);
void opl2_setInstrument(uint8_t channel, instrument_t *instrument, float volume);
void opl2_setKeyOn(uint8_t channel, bool keyOn);
void opl2_setMaintainSustain(uint8_t channel, uint8_t operatorNum, bool enable);
void opl2_setMultiplier(uint8_t channel, uint8_t operatorNum, uint8_t multiplier);
void opl2_setNoteSelect(bool enable);
void opl2_setPercussion(bool enable);
void opl2_setRelease(uint8_t channel, uint8_t operatorNum, uint8_t release);
void opl2_setScalingLevel(uint8_t channel, uint8_t operatorNum, uint8_t scaling);
void opl2_setSustain(uint8_t channel, uint8_t operatorNum, uint8_t sustain);
void opl2_setSynthMode(uint8_t channel, uint8_t synthMode);
void opl2_setTremolo(uint8_t channel, uint8_t operatorNum, bool enable);
void opl2_setVibrato(uint8_t channel, uint8_t operatorNum, bool enable);
void opl2_setVolume(uint8_t channel, uint8_t operatorNum, uint8_t volume);
void opl2_setWaveFormSelect(bool enable);

#ifdef __cplusplus
}
#endif
#endif  // __OPL2_H_
