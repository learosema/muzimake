/**
 * ROL File
 * implemented according to the documentation from here:
 * https://moddingwiki.shikadi.net/wiki/ROL_Format
 * kudos to Chris Holmes and binarymaster
 * for reverse engineering it <3
 */
#ifndef __ROLFILE_H__
#define __ROLFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NUM_VOICE_TRACKS 11
#define NUM_TIMBRE_TRACKS 11
#define NUM_VOLUME_TRACKS 11
#define NUM_PITCH_TRACKS 11
#define LEN_TRACK_NAME 15
#define LEN_INSTRUMENT_NAME 9
#define NUM_TRACKS (1 + NUM_VOICE_TRACKS + NUM_TIMBRE_TRACKS + NUM_VOLUME_TRACKS + NUM_PITCH_TRACKS)

typedef struct __rol_counters {
	uint16_t numTicks[NUM_VOICE_TRACKS];
	uint16_t numTimbreEvents[NUM_TIMBRE_TRACKS];
	uint16_t numVolumeEvents[NUM_VOLUME_TRACKS];
	uint16_t numPitchEvents[NUM_PITCH_TRACKS];
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
	rol_counters_t counters;
	uint8_t  filler[38]; /** filler zero-bytes */
} rol_header_t;

typedef struct __rol_tempo_event {
	uint16_t atTick; /** time of event in ticks */
	float multiplier; /** tempo multiplier (in range 0.01 - 10.0) */
} rol_tempo_event_t;

typedef struct __rol_tempo_track {
	char trackName[LEN_TRACK_NAME]; /** track name, 0-terminated, usually "tempo" */
	float basicTempo; /** song tempo, in BPM */
	uint16_t numEvents; /** number of events */
	rol_tempo_event_t *tempoEvents; /** array of tempo events */
} rol_tempo_track_t;

typedef struct __rol_note_event {
	uint16_t note; /** note number, 0=off, 12..127=note on, 48=middle c */
	uint16_t duration; /** note duration, in ticks */
} rol_note_event_t;

typedef struct __rol_voice_track {
	char trackName[LEN_TRACK_NAME]; /** usually "Voix ## (voice no from 0)" */
	uint16_t numTicks;
	rol_note_event_t *noteEvents;
} rol_voice_track_t;

typedef struct __rol_timbre_event {
	uint16_t atTick; /** time of event, in ticks */
	char instrument[LEN_INSTRUMENT_NAME]; /** instrument name, 0-terminated */
	uint8_t filler; /* 0-byte filler */
	uint16_t unknown; /** sometimes equal to instrument index */
} rol_timbre_event_t;

typedef struct __rol_timbre_track {
	char trackName[LEN_TRACK_NAME]; /** trackname, usually "Timbre ##" */
	uint16_t numEvents; /** number of Events */
	rol_timbre_event_t *timbreEvents; /** array of timbre events */
} rol_timbre_track_t;

typedef struct __rol_volume_event {
	uint16_t atTick; /** time of event, in ticks */
	float volume; /** volume multiplier (0..1) */
} rol_volume_event_t;

typedef struct __rol_volume_track {
	char trackName[LEN_TRACK_NAME]; /** trackname, 0-term, usually "Volume ##" */
	uint16_t numEvents; /** number of events */
	rol_volume_event_t *volumeEvents; /** array of volume events */
} rol_volume_track_t;

typedef struct __rol_pitch_event {
	uint16_t atTick; /** Time of event, in ticks */
	float pitch; /** Pitch variation (in range 0.0 - 2.0), nominal pitch value is 1.0 */
} rol_pitch_event_t;

typedef struct __rol_pitch_track {
	char trackName[LEN_TRACK_NAME]; /** track name, 0 term, usually "Pitch ##" */
	uint16_t numEvents; /** number of events */
	rol_pitch_event_t *pitchEvents; /** Array of pitch events */
} rol_pitch_track_t;

typedef struct __rol_file {
	rol_header_t header;
	rol_tempo_track_t tempoTrack;
	rol_voice_track_t voiceTrack[NUM_VOICE_TRACKS];
	rol_timbre_track_t timbreTrack[NUM_TIMBRE_TRACKS];
	rol_volume_track_t volumeTrack[NUM_VOLUME_TRACKS];
	rol_pitch_track_t pitchTrack[NUM_PITCH_TRACKS];
} rol_file_t;

bool rolfile_read(rol_file_t *rolFile, char *filename);
void rolfile_dispose(rol_file_t *rolFile);

#ifdef __cplusplus
}
#endif
#endif
