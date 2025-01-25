#include <stdio.h>
#include "bnkfile.h"
#include "rolfile.h"






int main() {
	rol_file_t rol;
	if (! rolfile_read(&rol, "STARTREK.ROL")) {
		fprintf(stderr, "Could not load STARTREK.ROL :(\n");
	}
	printf("ROL Header size...: %d\n", sizeof(rol_header_t));

	printf("isMelodic.........: %s\n", rol.header.isMelodic ? "true" : "false");
	printf("tickBeat..........: %d\n", rol.header.tickBeat);
	printf("beatMeasure.......: %d\n", rol.header.beatMeasure);
	printf("Tempo trackname...: %s\n", rol.tempoTrack.trackName);
	printf("Num Tempo Events..: %d\n", rol.tempoTrack.numEvents);
	printf("basicTempo(BPM)...: %f\n", rol.tempoTrack.basicTempo);
	printf("tempo events......: %d\n", rol.tempoTrack.numEvents);




	rolfile_dispose(&rol);
}
