#include <stdlib.h>
#ifdef __DOS__
#include <dos.h>
#endif
#include "file.h"
#include "rolfile.h"


#define ROL_ERR_READ_FAILED 1

#define ALLOC_TYPE(T, N) ((T *)malloc(sizeof(T) * N))

int rol_last_error = 0;

rol_file_t * rolfile_read(char * filename)
{
	FILEPTR fp;
	rol_file_t *rolFile = NULL;
	long len;

	fp = file_open(filename, "rb");
	long len = file_get_size(fp);


	rolFile = ALLOC_TYPE(rol_file_t, 1);

	file_read(&(rolFile->header), sizeof(rol_header_t), 1, fp);

	file_read_chars(fp, rolFile->tempoTrack.trackName, LEN_TRACK_NAME);

	rolFile->tempoTrack.numEvents = file_read_u16le(fp);
	rolFile->tempoTrack.tempoEvents = ALLOC_TYPE(rol_tempo_event_t, rolFile->tempoTrack.numEvents);
	for (int i = 0; i < rolFile->tempoTrack.numEvents; i++) {
		rol_tempo_event_t event;
		event.atTick = file_read_u16le(fp);
		event.multiplier = file_read_f32le(fp);
		rolFile->tempoTrack.tempoEvents[i] = event;
	}

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

for (int j = 0; j < NUM_TIMBRE_TRACKS; j++) {
		file_read_chars(fp, rolFile->timbreTrack[j].trackName, LEN_TRACK_NAME);
		rolFile->voiceTrack[j].numTicks = file_read_u16le(fp);
		uint16_t numTicks = rolFile->timbreTrack[j].numEvents;

		rolFile->timbreTrack[j].timbreEvents = ALLOC_TYPE(rol_timbre_event_t, numTicks);
		for (uint16_t i = 0; i < numTicks; i++) {
			rol_timbre_event_t event;
			// event.atTick =

		}
	}




	fclose(fp);
	return rolFile;
}
