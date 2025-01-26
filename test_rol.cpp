#include <stdio.h>
#include "bnkfile.h"
#include "rolfile.h"

#ifdef __DOS__
#include <conio>
#else
int getch() { /* stub */ }
#endif

int main() {
	rol_file_t rol;
	if (! rolfile_read(&rol, "STARTREK.ROL")) {
		fprintf(stderr, "Could not load STARTREK.ROL :(\n");
	}
	printf("ROL Header size...: %d\n", sizeof(rol_header_t));

	printf("isMelodic.........: %s\n", rol.header.isMelodic ? "true" : "false");
	printf("tickBeat..........: %d\n", rol.header.tickBeat);
	printf("beatMeasure.......: %d\n", rol.header.beatMeasure);

	getch();
	printf("Tempo trackname...: %s\n", rol.tempoTrack.trackName);
	printf("Num Tempo Events..: %d\n", rol.tempoTrack.numEvents);
	printf("basicTempo(BPM)...: %f\n", rol.tempoTrack.basicTempo);
	printf("tempo events......: %d\n", rol.tempoTrack.numEvents);

	getch();

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

	getch();

	printf("Timbre tracks.....:\n");
	for (uint16_t i = 0; i < 11; i++) {
		uint16_t numEvents = rol.timbreTrack[i].numEvents;
		printf(" '%s': %d timbre events =>", rol.timbreTrack[i].trackName, numEvents);
		for (uint16_t j = 0; j < numEvents; j++) {
			printf(" (%d: '%s')", rol.timbreTrack[i].timbreEvents[j].atTick, rol.timbreTrack[i].timbreEvents[j].instrument);
		}
		printf("\n");
	}

	getch();

	printf("Volume tracks.....:\n");
	for (uint16_t i = 0; i < 11; i++) {
		uint16_t numEvents = rol.volumeTrack[i].numEvents;
		printf(" '%s': %d volume events =>", rol.volumeTrack[i].trackName, NUM_VOLUME_TRACKS);
		for (uint16_t j = 0; j < numEvents; j++) {
			printf(" (%d: %.2f)", rol.volumeTrack[i].volumeEvents[j].atTick,  rol.volumeTrack[i].volumeEvents[j].volume);
		}
		printf("\n");
	}
	getch();

	printf("Pitch tracks......:\n");
	for (uint16_t i = 0; i < 11; i++) {
		uint16_t numEvents = rol.pitchTrack[i].numEvents;
		printf(" '%s': %d pitch events =>", rol.pitchTrack[i].trackName, NUM_VOLUME_TRACKS);
		for (uint16_t j = 0; j < numEvents; j++) {
			printf(" (%d: %.2f)", rol.pitchTrack[i].pitchEvents[j].atTick, rol.pitchTrack[i].pitchEvents[j].pitch);
		}
		printf("\n");
	}

	rolfile_dispose(&rol);
}
