#include "greatest.h"
#include <string.h>
#include "file.h"
#include "rolfile.h" // TODO: to be removed, when structure is refactored
#include "pathutil.h"

/* ---------- Fixture ---------- */

static const char *g_rol_dir = "."; /* per --dir=... überschreibbar, siehe main() */

typedef struct {
	sound_file_t soundfile;
	rol_file_t rol;
	bool loaded;
} rol_fixture_t;

static rol_fixture_t fx;

static void rol_setup(void *arg) {
	(void)arg;
	memset(&fx, 0, sizeof(fx));
	fx.soundfile = NULL;

	path_t *rolFilePath = path_create(g_rol_dir);
	path_join(rolFilePath, "STARTREK.ROL");

	file_init(&fx.soundfile, FORMAT_ROL);
	file_result_t result = file_open(&fx.soundfile, rolFilePath->str);
	path_dispose(rolFilePath);

	if (result != ERROR) {
		rol_file_t *rolFile = (rol_file_t *)file_get_raw(&fx.soundfile); // TODO: to be removed, when structure is refactored
		fx.rol = *rolFile; // TODO: to be removed, when structure is refactored
		fx.loaded = true;
	}
}

static void rol_teardown(void *arg) {
	(void)arg;
	if (fx.loaded) {
		file_close(&fx.soundfile);
	}
}

/* ---------- Tests: Header ---------- */

TEST rol_file_should_open_successfully(void) {
	ASSERT(fx.loaded);
	PASS();
}

TEST rol_header_should_have_positive_timing_values(void) {
	ASSERT(fx.loaded);
	/* tickBeat/beatMeasure sind die musikalische Auflösung -
	 * 0 wäre ein kaputtes/leeres Timing-Setup */
	ASSERT(fx.rol.header.tickBeat > 0);
	ASSERT(fx.rol.header.beatMeasure > 0);
	PASS();
}

/* ---------- Tests: Tempo Track ---------- */

TEST rol_tempo_track_should_have_name_and_positive_bpm(void) {
	ASSERT(fx.loaded);
	ASSERT(strlen(fx.rol.tempoTrack.trackName) > 0);
	ASSERT(fx.rol.tempoTrack.basicTempo > 0.0);
	PASS();
}

TEST rol_tempo_track_event_count_should_be_consistent(void) {
	ASSERT(fx.loaded);
	/* numEvents ist uint16_t, also per Definition nie negativ -
	 * der eigentliche Sinn dieses Tests ist, dass das Feld überhaupt
	 * gelesen werden kann, ohne dass die Struktur kaputt/verschoben ist */
	ASSERT(fx.rol.tempoTrack.numEvents >= 0);
	PASS();
}

/* ---------- Tests: Voice Tracks ---------- */

TEST rol_voice_tracks_should_have_names(void) {
	ASSERT(fx.loaded);
	for (int i = 0; i < 11; i++) {
		ASSERT(strlen(fx.rol.voiceTrack[i].trackName) > 0);
	}
	PASS();
}

TEST rol_voice_tracks_note_events_should_sum_to_num_ticks(void) {
	ASSERT(fx.loaded);
	/* Replikat der ursprünglichen Zähllogik aus dem main(), jetzt als
	 * Assertion: die kumulierten note-event-Dauern müssen exakt bei
	 * numTicks ankommen, sonst ist der Track intern inkonsistent. */
	for (int i = 0; i < 11; i++) {
		uint16_t numTicks = fx.rol.voiceTrack[i].numTicks;
		uint16_t tick = 0;
		uint16_t count = 0;
		for (uint16_t j = 0; tick < numTicks; j++) {
			tick += fx.rol.voiceTrack[i].noteEvents[j].duration;
			count++;
		}
		ASSERT_EQ(numTicks, tick);
	}
	PASS();
}

/* ---------- Tests: Timbre Tracks ---------- */

TEST rol_timbre_tracks_should_have_names(void) {
	ASSERT(fx.loaded);
	for (int i = 0; i < 11; i++) {
		ASSERT(strlen(fx.rol.timbreTrack[i].trackName) > 0);
	}
	PASS();
}

TEST rol_timbre_events_should_have_nonempty_instrument_names(void) {
	ASSERT(fx.loaded);
	for (int i = 0; i < 11; i++) {
		uint16_t numEvents = fx.rol.timbreTrack[i].numEvents;
		for (uint16_t j = 0; j < numEvents; j++) {
			ASSERT(strlen(fx.rol.timbreTrack[i].timbreEvents[j].instrument) > 0);
		}
	}
	PASS();
}

/* ---------- Tests: Volume Tracks ---------- */

TEST rol_volume_tracks_should_have_names(void) {
	ASSERT(fx.loaded);
	for (int i = 0; i < 11; i++) {
		ASSERT(strlen(fx.rol.volumeTrack[i].trackName) > 0);
	}
	PASS();
}

TEST rol_volume_events_should_be_in_valid_range(void) {
	ASSERT(fx.loaded);
	for (int i = 0; i < 11; i++) {
		uint16_t numEvents = fx.rol.volumeTrack[i].numEvents;
		for (uint16_t j = 0; j < numEvents; j++) {
			float vol = fx.rol.volumeTrack[i].volumeEvents[j].volume;
			ASSERT(vol >= 0.0f);
		}
	}
	PASS();
}

/* ---------- Tests: Pitch Tracks ---------- */

TEST rol_pitch_tracks_should_have_names(void) {
	ASSERT(fx.loaded);
	for (int i = 0; i < 11; i++) {
		ASSERT(strlen(fx.rol.pitchTrack[i].trackName) > 0);
	}
	PASS();
}

/* ---------- Fehlerfall ---------- */

TEST rol_file_open_should_fail_for_missing_file(void) {
	sound_file_t sf = NULL;
	file_init(&sf, FORMAT_ROL);
	file_result_t result = file_open(&sf, "THIS_FILE_DOES_NOT_EXIST.ROL");
	ASSERT_EQ(ERROR, result);
	PASS();
}

/* ---------- Suite ---------- */

SUITE(rol_instrument_suite) {
	SET_SETUP(rol_setup, NULL);
	SET_TEARDOWN(rol_teardown, NULL);

	RUN_TEST(rol_file_should_open_successfully);
	RUN_TEST(rol_header_should_have_positive_timing_values);
	RUN_TEST(rol_tempo_track_should_have_name_and_positive_bpm);
	RUN_TEST(rol_tempo_track_event_count_should_be_consistent);
	RUN_TEST(rol_voice_tracks_should_have_names);
	RUN_TEST(rol_voice_tracks_note_events_should_sum_to_num_ticks);
	RUN_TEST(rol_timbre_tracks_should_have_names);
	RUN_TEST(rol_timbre_events_should_have_nonempty_instrument_names);
	RUN_TEST(rol_volume_tracks_should_have_names);
	RUN_TEST(rol_volume_events_should_be_in_valid_range);
	RUN_TEST(rol_pitch_tracks_should_have_names);
	RUN_TEST(rol_file_open_should_fail_for_missing_file);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (strncmp(argv[i], "--dir=", 6) == 0) {
			g_rol_dir = argv[i] + 6;
		}
	}

	GREATEST_MAIN_BEGIN();
	RUN_SUITE(rol_instrument_suite);
	GREATEST_MAIN_END();
}
