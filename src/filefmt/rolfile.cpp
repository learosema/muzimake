#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __DOS__
#include <dos.h>
#endif

#include <macros.h>

#include "fileio.h"
#include "rolfile.h"



int rol_last_error = 0;

/**
 * Read tempo track. Allocates memory for the tempoEvents array.
 */
void rolfile_read_tempo_track(FILEPTR fp, rol_file_t *rolFile)
{
	fileio_read_chars(fp, rolFile->tempoTrack.trackName, LEN_TRACK_NAME);
	rolFile->tempoTrack.basicTempo = fileio_read_f32le(fp);
	rolFile->tempoTrack.numEvents = fileio_read_u16le(fp);
	rolFile->tempoTrack.tempoEvents = ALLOC_TYPE(rol_tempo_event_t, rolFile->tempoTrack.numEvents);
	for (int i = 0; i < rolFile->tempoTrack.numEvents; i++) {
		rol_tempo_event_t event;
		event.atTick = fileio_read_u16le(fp);
		event.multiplier = fileio_read_f32le(fp);
		rolFile->tempoTrack.tempoEvents[i] = event;
	}
}

/**
 * Read voice tracks. Allocates memory for the noteEvents arrays.
 */
void rolfile_read_voice_tracks(FILEPTR fp, rol_file_t *rolFile, uint16_t trackIndex)
{
	fileio_read_chars(fp, rolFile->voiceTrack[trackIndex].trackName, LEN_TRACK_NAME);

	rolFile->voiceTrack[trackIndex].numTicks = fileio_read_u16le(fp);

	uint16_t numTicks = rolFile->voiceTrack[trackIndex].numTicks;
	uint16_t count = 0;
	rol_note_event_t *events = ALLOC_TYPE(rol_note_event_t, numTicks);
	for (uint16_t i = 0, tick = 0; tick < numTicks; i++) {
		rol_note_event_t event;
		event.note = fileio_read_u16le(fp);
		event.duration = fileio_read_u16le(fp);
		events[i] = event;

		tick += event.duration;
		count++;
	}
	rolFile->voiceTrack[trackIndex].noteEvents = REALLOC_TYPE(events, rol_note_event_t, count);
	rolFile->voiceTrack[trackIndex].numEvents = count;
}

/**
 * Read timbre tracks and allocate memory for events
 */
void rolfile_read_timbre_track(FILEPTR fp, rol_file_t *rolFile, uint16_t trackIndex)
{
	fileio_read_chars(fp, rolFile->timbreTrack[trackIndex].trackName, LEN_TRACK_NAME);

	uint16_t numEvents = fileio_read_u16le(fp);
	rolFile->timbreTrack[trackIndex].numEvents = numEvents;

	rolFile->timbreTrack[trackIndex].timbreEvents = ALLOC_TYPE(rol_timbre_event_t, numEvents);
	for (uint16_t i = 0; i < numEvents; i++) {
		rol_timbre_event_t event;
		event.atTick = fileio_read_u16le(fp);
		fileio_read_chars(fp, event.instrument, 9);
		event.filler = fileio_read_u8(fp);
		event.unknown = fileio_read_u16le(fp);

		rolFile->timbreTrack[trackIndex].timbreEvents[i] = event;
	}
}

/**
 * Read volume tracks and allocate memory for events
 */
void rolfile_read_volume_track(FILEPTR fp, rol_file_t *rolFile, uint16_t trackIndex)
{
	fileio_read_chars(fp, rolFile->volumeTrack[trackIndex].trackName, LEN_TRACK_NAME);
	uint16_t numEvents = fileio_read_u16le(fp);
	rolFile->volumeTrack[trackIndex].numEvents = numEvents;
	rolFile->volumeTrack[trackIndex].volumeEvents = ALLOC_TYPE(rol_volume_event_t, numEvents);

	for (uint16_t i = 0; i < numEvents; i++) {
		rol_volume_event_t event;
		event.atTick = fileio_read_u16le(fp);
		event.volume = fileio_read_f32le(fp);
		rolFile->volumeTrack[trackIndex].volumeEvents[i] = event;
	}
}

/**
 * Read pitch tracks
 */
void rolfile_read_pitch_track(FILEPTR fp, rol_file_t *rolFile, uint16_t trackIndex)
{
	fileio_read_chars(fp, rolFile->pitchTrack[trackIndex].trackName, LEN_TRACK_NAME);
	uint16_t numEvents = fileio_read_u16le(fp);
	rolFile->pitchTrack[trackIndex].numEvents = numEvents;
	rolFile->pitchTrack[trackIndex].pitchEvents = ALLOC_TYPE(rol_pitch_event_t, numEvents);
	for (uint16_t i = 0; i < numEvents; i++) {
		rol_pitch_event_t event;
		event.atTick = fileio_read_u16le(fp);
		event.pitch = fileio_read_f32le(fp);

		rolFile->pitchTrack[trackIndex].pitchEvents[i] = event;
	}
}

void rolfile_init(file_data_t* file)
{
	file->type = MUSIC;
}

void rolfile_read(file_data_t* file, char* filename)
{
	FILEPTR fp = fileio_open(filename, "rb");
	long len = fileio_get_size(fp);
	if (len < sizeof(rol_header_t)) {
		return;
	}

	rol_file_t* rolFile = (rol_file_t *)malloc(sizeof(rol_file_t));

	fileio_read(&(rolFile->header), sizeof(rol_header_t), 1, fp);

	if (strncmp(rolFile->header.signature, "\\roll\\default", 13) != 0) {
		return;
	}

	rolfile_read_tempo_track(fp, rolFile);

	for (uint16_t trackIndex = 0; trackIndex < 11; trackIndex++) {
		rolfile_read_voice_tracks(fp, rolFile, trackIndex);
		rolfile_read_timbre_track(fp, rolFile, trackIndex);
		rolfile_read_volume_track(fp, rolFile, trackIndex);
		rolfile_read_pitch_track(fp, rolFile, trackIndex);
	}

	fileio_close(fp);

	file->data = rolFile; // TODO: to be removed, when structure is refactored
}

bool rolfile_write(file_data_t* file, char *filename)
{
	return false;
}

void rolfile_free(file_data_t* file)
{
	rol_file_t* rolFile = (rol_file_t*)file->data; // TODO: to be removed, when structure is refactored

	free(rolFile->tempoTrack.tempoEvents);
	for (int i = 0; i < NUM_VOICE_TRACKS; i++) {
		free(rolFile->voiceTrack[i].noteEvents);
		free(rolFile->timbreTrack[i].timbreEvents);
		free(rolFile->volumeTrack[i].volumeEvents);
		free(rolFile->pitchTrack[i].pitchEvents);
	}
	memset(rolFile, 0, sizeof(rol_file_t));
	free(rolFile);
}

void rolfile_debug(file_data_t* file)
{
	//
}
