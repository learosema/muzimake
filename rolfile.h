/**
 * ROL File
 * implemented according to the documentation from here:
 * https://moddingwiki.shikadi.net/wiki/ROL_Format
 *
 */
#ifndef __ROLFILE_H__
#define __ROLFILE_H__
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct __rol_counters {
	uint16_t numTicks[11];
	uint16_t numTimbreEvents[11];
	uint16_t numVolumeEvents[11];
	uint16_t numPitchEvents[11];
	uint16_t numTempoEvents;
} rol_counters_t;

/**
 * ROL Metadata
 */
typedef struct __rol_header
{
	uint16_t majorVersion; /** usually 0 */
	uint16_t minorVersion; /** usually 4 */
	char     signature[40]; /* usually "\roll\default" */
	uint16_t tickBeat; /** ticks per beat */
	uint16_t beatMeasure; /** beats per measure */
	uint16_t scaleY; /** used by editor */
	uint16_t scaleX; /** used by editor */
	uint8_t  reserved; /** unused, set to 0 */
	uint8_t  isMelodic; /** 1 if melodic, 0 if percussions are used */
	rol_counters_t counters[45];
	uint8_t  filler[38]; /** filler zero-bytes */
} rol_header_t;

typedef struct __rol_tempo_event {
	uint16_t atTick; /** time of event in ticks */
	float multiplier; /** tempo multiplier (in range 0.01 - 10.0) */
} rol_tempo_event_t;

typedef struct __rol_tempo_track {
	char trackName[15]; /** track name, 0-terminated, usually "tempo" */
	float basicTempo; /** song tempo, in BPM */
	uint16_t numEvents; /** number of events */
	rol_tempo_event_t *tempoEvents; /** array of tempo events */
} rol_tempo_track_t;

typedef struct __rol_note_event {
	uint16_t note; /** note number, 0=off, 12..127=note on, 48=middle c */
	uint16_t duration; /** note duration, in ticks */
} rol_note_event_t;

typedef struct __rol_voice_track {
	char trackName[15]; /** usually "Voix ## (voice no from 0)" */
	uint16_t numTicks;
	rol_note_event_t *noteEvents;
} rol_voice_track_t;

typedef struct __rol_timbre_event {
	uint16_t atTick; /** time of event, in ticks */
	char instrument[9]; /** instrument name, 0-terminated */
	uint8_t filler; /* 0-byte filler */
	uint16_t unknown; /** sometimes equal to instrument index */
} rol_timbre_event_t;

typedef struct __rol_timbre_track {
	char trackName[15]; /** trackname, usually "Timbre ##" */
	uint16_t numEvents; /** number of Events */
	rol_timbre_event_t *timbreEvents; /** array of timbre events */
} rol_timbre_track_t;

typedef struct __rol_volume_event {
	uint16_t atTick; /** time of event, in ticks */
	float volume; /** volume multiplier (0..1) */
} rol_volume_event_t;

typedef struct __rol_volume_track {
	char trackName[15]; /** trackname, 0-term, usually "Volume ##" */
	uint16_t numEvents; /** number of events */
	rol_volume_event_t *volumeEvents; /** array of volume events */
} rol_volume_track_t;

typedef struct __rol_pitch_event {
	uint16_t atTick; /** Time of event, in ticks */
	float pitch; /** Pitch variation (in range 0.0 - 2.0), nominal pitch value is 1.0 */
} rol_pitch_event_t;

typedef struct __rol_pitch_track {
	char trackName[15]; /** track name, 0 term, usually "Pitch ##" */
	uint16_t numEvents; /** number of events */
	rol_pitch_event_t *pitchEvents; /** Array of pitch events */
} rol_pitch_track_t;

typedef struct __rol_file {
	rol_header_t header;
	rol_tempo_track_t tempoTrack;
	rol_voice_track_t voiceTrack;
	rol_timbre_track_t timbreTrack;
	rol_volume_event_t volumeTrack;
	rol_pitch_track_t pitchTrack;
} rol_file_t;

#ifdef __cplusplus
}
#endif

#endif
