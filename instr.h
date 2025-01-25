#ifndef __INSTR_H__
#define __INSTR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Generic OPL2 definitions.
#define OPL2_NUM_CHANNELS 9
#define OPL2_CHANNELS_PER_BANK 9

// Operator definitions.
#define OPL2_OPERATOR1 0
#define OPL2_OPERATOR2 1
#define OPL2_MODULATOR 0
#define OPL2_CARRIER 1

// Synthesis type definitions.
#define OPL2_SYNTH_MODE_FM 0
#define OPL2_SYNTH_MODE_AM 1

// Drum sounds.
#define OPL2_DRUM_BASS 0
#define OPL2_DRUM_SNARE 1
#define OPL2_DRUM_TOM 2
#define OPL2_DRUM_CYMBAL 3
#define OPL2_DRUM_HI_HAT 4

// Drum sound bit masks.
#define OPL2_DRUM_BITS_BASS 0x10
#define OPL2_DRUM_BITS_SNARE 0x08
#define OPL2_DRUM_BITS_TOM 0x04
#define OPL2_DRUM_BITS_CYMBAL 0x02
#define OPL2_DRUM_BITS_HI_HAT 0x01

// Note to frequency mapping.
#define OPL2_NOTE_C 0
#define OPL2_NOTE_CS 1
#define OPL2_NOTE_D 2
#define OPL2_NOTE_DS 3
#define OPL2_NOTE_E 4
#define OPL2_NOTE_F 5
#define OPL2_NOTE_FS 6
#define OPL2_NOTE_G 7
#define OPL2_NOTE_GS 8
#define OPL2_NOTE_A 9
#define OPL2_NOTE_AS 10
#define OPL2_NOTE_B 11

// Tune specific declarations.
#define OPL2_NUM_OCTAVES 7
#define OPL2_NUM_NOTES 12
#define OPL2_NUM_DRUM_SOUNDS 5

typedef struct __operator {
    bool hasTremolo;
    bool hasVibrato;
    bool hasSustain;
    bool hasEnvelopeScaling;
    uint8_t frequencyMultiplier;
    uint8_t keyScaleLevel;
    uint8_t outputLevel;
    uint8_t attack;
    uint8_t decay;
    uint8_t sustain;
    uint8_t release;
    uint8_t waveForm;
} operator_t;

typedef struct __instrument {
    operator_t operators[2];
    uint8_t feedback;
    bool isAdditiveSynth;
		bool isPercussive;
		uint8_t drumType;
    uint8_t transpose;
} instrument_t;


#ifdef __cplusplus
}
#endif

#endif
