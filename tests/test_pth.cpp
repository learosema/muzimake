#include <greatest.h>
#include <stdio.h>
#include "pathutil.h"

TEST path_join_should_concatenate_two_paths()
{
	path_t *path = path_create(".");
	path_join(path, "..");

	ASSERT_EQm("path '.' and '..' should be joined to './..'", strcmp(path->str, "./.."), 0);
	path_dispose(path);
	PASS();
}

SUITE(pathutil_tests)
{
	RUN_TEST(path_join_should_concatenate_two_paths);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(pathutil_tests);
    GREATEST_MAIN_END();        /* display results */
}
