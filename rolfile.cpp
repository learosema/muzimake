#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef __DOS__
#include <dos.h>
#endif
#include "file.h"
#include "rolfile.h"

#define ALLOC_TYPE(T, N) ((T *)malloc(sizeof(T) * N))

int rol_last_error = 0;

/**
 * Read tempo track. Allocates memory for the tempoEvents array.
 */
void rolfile_read_tempo_track(FILEPTR fp, rol_file_t *rolFile)
{
	file_read_chars(fp, rolFile->tempoTrack.trackName, LEN_TRACK_NAME);
	rolFile->tempoTrack.basicTempo = file_read_f32le(fp);
	rolFile->tempoTrack.numEvents = file_read_u16le(fp);
	rolFile->tempoTrack.tempoEvents = ALLOC_TYPE(rol_tempo_event_t, rolFile->tempoTrack.numEvents);
	for (int i = 0; i < rolFile->tempoTrack.numEvents; i++) {
		rol_tempo_event_t event;
		event.atTick = file_read_u16le(fp);
		event.multiplier = file_read_f32le(fp);
		rolFile->tempoTrack.tempoEvents[i] = event;
	}
}

/**
 * Read voice tracks. Allocates memory for the noteEvents arrays.
 */
void rolfile_read_voice_tracks(FILEPTR fp, rol_file_t *rolFile)
{
	for (int j = 0; j < NUM_VOICE_TRACKS; j++) {
		file_read_chars(fp, rolFile->voiceTrack[j].trackName, LEN_TRACK_NAME);
		rolFile->voiceTrack[j].numTicks = file_read_u16le(fp);
		uint16_t numTicks = rolFile->voiceTrack[j].numTicks;

		rolFile->voiceTrack[j].noteEvents = ALLOC_TYPE(rol_note_event_t, numTicks);
		for (uint16_t i = 0; i < numTicks; i++) {
			rol_note_event_t event;
			event.note = file_read_u16le(fp);
			event.duration = file_read_u16le(fp);

			rolFile->voiceTrack[j].noteEvents[i] = event;
		}
	}
}

/**
 * Read timbre tracks and allocate memory for events
 */
void rolfile_read_timbre_tracks(FILEPTR fp, rol_file_t *rolFile)
{
	for (int j = 0; j < NUM_TIMBRE_TRACKS; j++) {
		file_read_chars(fp, rolFile->timbreTrack[j].trackName, LEN_TRACK_NAME);
		rolFile->voiceTrack[j].numTicks = file_read_u16le(fp);
		uint16_t numEvents = rolFile->timbreTrack[j].numEvents;

		rolFile->timbreTrack[j].timbreEvents = ALLOC_TYPE(rol_timbre_event_t, numEvents);
		for (uint16_t i = 0; i < numEvents; i++) {
			rol_timbre_event_t event;
			event.atTick = file_read_u16le(fp);
			file_read_chars(fp, event.instrument, 9);
			event.filler = file_read_u8(fp);
			event.unknown = file_read_u16le(fp);

			rolFile->timbreTrack[j].timbreEvents[i] = event;
		}
	}
}

/**
 * Read volume tracks and allocate memory for events
 */
void rolfile_read_volume_tracks(FILEPTR fp, rol_file_t *rolFile)
{
	for (int j = 0; j < NUM_VOLUME_TRACKS; j++) {
		file_read_chars(fp, rolFile->volumeTrack[j].trackName, LEN_TRACK_NAME);
		rolFile->volumeTrack[j].numEvents = file_read_u16le(fp);
		uint16_t numEvents = rolFile->volumeTrack[j].numEvents;
		rolFile->volumeTrack[j].volumeEvents = ALLOC_TYPE(rol_volume_event_t, numEvents);

		for (uint16_t i = 0; i < numEvents; i++) {
			rol_volume_event_t event;
			event.atTick = file_read_u16le(fp);
			event.volume = file_read_f32le(fp);
			rolFile->volumeTrack[j].volumeEvents[i] = event;
		}
	}
}

/**
 * Read pitch tracks
 */
void rolfile_read_pitch_tracks(FILEPTR fp, rol_file_t *rolFile)
{
	for (int j = 0; j < NUM_PITCH_TRACKS; j++) {
		file_read_chars(fp, rolFile->pitchTrack[j].trackName, LEN_TRACK_NAME);
		uint16_t numEvents = file_read_u16le(fp);
		rolFile->pitchTrack[j].numEvents = numEvents;
		rolFile->pitchTrack[j].pitchEvents = ALLOC_TYPE(rol_pitch_event_t, numEvents);
		for (uint16_t i = 0; i < numEvents; i++) {
			rol_pitch_event_t event;
			event.atTick = file_read_u16le(fp);
			event.pitch = file_read_f32le(fp);

			rolFile->pitchTrack[j].pitchEvents[i] = event;
		}
	}
}

/**
 * Read ROL file
 */
bool rolfile_read(rol_file_t *rolFile, char *filename)
{
	FILEPTR fp = file_open(filename, "rb");
	long len = file_get_size(fp);
	if (len < sizeof(rol_header_t)) {
		return false;
	}

	file_read(&(rolFile->header), sizeof(rol_header_t), 1, fp);

	if (strncmp(rolFile->header.signature, "\\roll\\default", 13) != 0) {
		return false;
	}

	rolfile_read_tempo_track(fp, rolFile);
	rolfile_read_voice_tracks(fp, rolFile);
	rolfile_read_timbre_tracks(fp, rolFile);
	rolfile_read_pitch_tracks(fp, rolFile);

	file_close(fp);
	return true;
}

void rolfile_dispose(rol_file_t *rolFile)
{
	free(rolFile->tempoTrack.tempoEvents);
	for (int i = 0; i < 11; i++) {
		free(rolFile->voiceTrack[i].noteEvents);
		free(rolFile->timbreTrack[i].timbreEvents);
		free(rolFile->volumeTrack[i].volumeEvents);
		free(rolFile->pitchTrack[i].pitchEvents);
	}
	memset(rolFile, 0, sizeof(rol_file_t));
}
