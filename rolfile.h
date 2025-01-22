#ifndef __ROLFILE_H__
#define __ROLFILE_H__
#include <stdint.h>

// implemented from here: https://moddingwiki.shikadi.net/wiki/ROL_Format


#ifdef __cplusplus
extern "C" {
#endif

typedef struct __rol_header {
	uint16_t majorVersion; // usually 0
	uint16_t minorVersion; // usually 4
	char     signature[40]; // usually "\roll\default"
	uint16_t tickBeat;
	uint16_t beatMeasure;
	uint16_t scaleY;
	uint16_t scaleX;
	uint8_t  reserved;
	uint8_t  isMelodic;
	uint16_t counters;
	uint8_t  filler[38];
} rol_header_t;

typedef struct __rol_tempo_event {
	uint16_t atTick;
	float tempoMultiplier;
} rol_tempo_event_t;

typedef struct __rol_tempo_track {
	char trackName[15]; // usually "tempo"
	float basicTempo;
	uint16_t numEvents;
	rol_tempo_event_t *tempoEvents;
} rol_tempo_track_t;


/*

char[15]	trackName	Track name, null terminated (usually "Tempo")
Single	basicTempo	Song tempo, in beats-per-minute
UINT16LE	nEvents	Number of events
E_TEMPO[nEvents]	tempoEvents	Array of tempo events
*/

typedef struct __rol_file {
	rol_header_t header;
	rol_tempo_track_t tempoTrack;
} rol_file_t;


#ifdef __cplusplus
}
#endif




#endif
