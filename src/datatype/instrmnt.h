#ifndef __INSTR_H__
#define __INSTR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

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

typedef struct __instrument_map_entry {
	char name[9];
	instrument_t instrument;
} instrument_map_entry_t;

typedef struct __instrument_map {
	uint16_t numItems;
	instrument_map_entry_t *entries;
} instrument_map_t;

instrument_map_t instrument_map_create(uint16_t numInstruments);
void instrument_map_sort(instrument_map_t *map);
void instrument_map_push(instrument_map_t * map, instrument_map_entry_t entry);
void instrument_map_resize(instrument_map_t * map, uint16_t numInstruments);
instrument_map_entry_t * instrument_map_find(instrument_map_t *map, const char name[9]);
void instrument_map_dispose(instrument_map_t *map);

#ifdef __cplusplus
}
#endif

#endif
