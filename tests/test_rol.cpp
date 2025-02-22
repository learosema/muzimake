#include <stdio.h>

#include <file.h>

#include "rolfile.h" // TODO: to be removed, when structure is refactored

int main() {
	sound_file_t soundfile = NULL;

	file_result_t result;
	result = file_init(&soundfile, FORMAT_ROL);

	result = file_open(&soundfile, "STARTREK.ROL");

	if(result == ERROR){
		printf("*** Error ***\n");
	}
	else {
		printf("*** Success ***\n");
	}

	rol_file_t* rolFile = (rol_file_t*)file_get_raw(&soundfile); // TODO: to be removed, when structure is refactored

	rol_file_t rol = *rolFile; // TODO: to be removed, when structure is refactored

	printf("ROL Header size...: %d\n", sizeof(rol_header_t));

	printf("isMelodic.........: %s\n", rol.header.isMelodic ? "true" : "false");
	printf("tickBeat..........: %d\n", rol.header.tickBeat);
	printf("beatMeasure.......: %d\n", rol.header.beatMeasure);

	printf("Tempo trackname...: %s\n", rol.tempoTrack.trackName);
	printf("Num Tempo Events..: %d\n", rol.tempoTrack.numEvents);
	printf("basicTempo(BPM)...: %f\n", rol.tempoTrack.basicTempo);
	printf("tempo events......: %d\n", rol.tempoTrack.numEvents);

	printf("Voice tracks......:\n");
	for (int i = 0; i < 11; i++) {
		printf(" '%s': %d ticks => ", rol.voiceTrack[i].trackName, rol.voiceTrack[i].numTicks);
		uint16_t count = 0;
		for (uint16_t j = 0, tick = 0; tick < rol.voiceTrack[i].numTicks; j++) {
			tick += rol.voiceTrack[i].noteEvents[j].duration;
			count++;
		}
		printf("%d note events\n", count);
	}

	printf("Timbre tracks.....:\n");
	for (uint16_t i = 0; i < 11; i++) {
		uint16_t numEvents = rol.timbreTrack[i].numEvents;
		printf(" '%s': %d timbre events =>", rol.timbreTrack[i].trackName, numEvents);
		for (uint16_t j = 0; j < numEvents; j++) {
			printf(" (%d: '%s')", rol.timbreTrack[i].timbreEvents[j].atTick, rol.timbreTrack[i].timbreEvents[j].instrument);
		}
		printf("\n");
	}

	printf("Volume tracks.....:\n");
	for (uint16_t i = 0; i < 11; i++) {
		uint16_t numEvents = rol.volumeTrack[i].numEvents;
		printf(" '%s': %d volume events =>", rol.volumeTrack[i].trackName, NUM_VOLUME_TRACKS);
		for (uint16_t j = 0; j < numEvents; j++) {
			printf(" (%d: %.2f)", rol.volumeTrack[i].volumeEvents[j].atTick,  rol.volumeTrack[i].volumeEvents[j].volume);
		}
		printf("\n");
	}

	printf("Pitch tracks......:\n");
	for (uint16_t i = 0; i < 11; i++) {
		uint16_t numEvents = rol.pitchTrack[i].numEvents;
		printf(" '%s': %d pitch events =>", rol.pitchTrack[i].trackName, NUM_VOLUME_TRACKS);
		for (uint16_t j = 0; j < numEvents; j++) {
			printf(" (%d: %.2f)", rol.pitchTrack[i].pitchEvents[j].atTick, rol.pitchTrack[i].pitchEvents[j].pitch);
		}
		printf("\n");
	}

	file_close(&soundfile);
}
