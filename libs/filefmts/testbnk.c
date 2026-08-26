#include "greatest.h"
#include <string.h>
#include "file.h"
#include "pathutil.h"
#include "bnkfile.h" // TODO: to be removed, when structure is refactored

/* ---------- Fixture ---------- */

static const char *g_bnk_dir = "."; /* per --dir=... überschreibbar, siehe main() */

typedef struct {
	sound_file_t soundfile;
	bnk_file_t *bnkFile;
	instrument_map_t map;
	bool loaded;
} bnk_fixture_t;

static bnk_fixture_t fx;

static void bnk_setup(void *arg) {
	(void)arg;
	memset(&fx, 0, sizeof(fx));
	fx.soundfile = NULL;

	path_t *bankFilePath = path_create(g_bnk_dir);
	path_join(bankFilePath, "STANDARD.BNK");

	file_init(&fx.soundfile, FORMAT_BNK);
	file_result_t result = file_open(&fx.soundfile, bankFilePath->str);
	path_dispose(bankFilePath);

	if (result != ERROR) {
		fx.bnkFile = (bnk_file_t *)file_get_raw(&fx.soundfile);
		fx.map = bnkfile_convert_to_map(fx.bnkFile);
		fx.loaded = true;
	}
}

static void bnk_teardown(void *arg) {
	(void)arg;
	if (fx.loaded) {
		file_close(&fx.soundfile);
	}
}

/* ---------- Tests ---------- */

TEST bnk_file_should_open_successfully(void) {
	ASSERT(fx.loaded);
	PASS();
}

TEST bnk_map_should_find_known_instrument(void) {
	ASSERT(fx.loaded);
	instrument_map_entry_t *entry = instrument_map_find(&fx.map, "BDRUM1");
	ASSERT(entry != NULL);
	ASSERT_STR_EQ("BDRUM1", entry->name);
	PASS();
}

TEST bnk_map_should_return_null_for_unknown_instrument(void) {
	ASSERT(fx.loaded);
	instrument_map_entry_t *entry = instrument_map_find(&fx.map, "NOT_A_REAL_INSTRUMENT");
	ASSERT_EQ(NULL, entry);
	PASS();
}

TEST bnk_map_push_should_increase_num_items(void) {
	ASSERT(fx.loaded);
	uint16_t before = fx.map.numItems;

	instrument_map_entry_t newEntry = {0};
	strcpy(newEntry.name, "000_test");
	instrument_map_push(&fx.map, newEntry);

	ASSERT_EQ(before + 1, fx.map.numItems);
	PASS();
}

TEST bnk_map_push_should_insert_at_front(void) {
	ASSERT(fx.loaded);

	instrument_map_entry_t newEntry = {0};
	strcpy(newEntry.name, "000_test");
	instrument_map_push(&fx.map, newEntry);

	ASSERT_STR_EQ("000_test", fx.map.entries[0].name);
	PASS();
}

TEST bnk_map_pushed_entry_should_be_findable(void) {
	ASSERT(fx.loaded);

	instrument_map_entry_t newEntry = {0};
	strcpy(newEntry.name, "000_test");
	instrument_map_push(&fx.map, newEntry);

	instrument_map_entry_t *found = instrument_map_find(&fx.map, "000_test");
	ASSERT(found != NULL);
	PASS();
}

TEST bnk_file_open_should_fail_for_missing_file(void) {
	sound_file_t sf = NULL;
	file_init(&sf, FORMAT_BNK);
	file_result_t result = file_open(&sf, "THIS_FILE_DOES_NOT_EXIST.BNK");
	ASSERT_EQ(ERROR, result);
	PASS();
}

/* ---------- Suite ---------- */

SUITE(bnk_instrument_suite) {
	SET_SETUP(bnk_setup, NULL);
	SET_TEARDOWN(bnk_teardown, NULL);

	RUN_TEST(bnk_file_should_open_successfully);
	RUN_TEST(bnk_map_should_find_known_instrument);
	RUN_TEST(bnk_map_should_return_null_for_unknown_instrument);
	RUN_TEST(bnk_map_push_should_increase_num_items);
	RUN_TEST(bnk_map_push_should_insert_at_front);
	RUN_TEST(bnk_map_pushed_entry_should_be_findable);
	RUN_TEST(bnk_file_open_should_fail_for_missing_file);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	/* eigenes --dir=... vor greatest herausfiltern, Rest an greatest weiterreichen */
	for (int i = 1; i < argc; i++) {
		if (strncmp(argv[i], "--dir=", 6) == 0) {
			g_bnk_dir = argv[i] + 6;
		}
	}

	GREATEST_MAIN_BEGIN();
	RUN_SUITE(bnk_instrument_suite);
	GREATEST_MAIN_END();
}
