#include <greatest.h>
#include <stdio.h>
#include "pathutil.h"

TEST path_join_should_not_crash_when_any_param_is_null()
{
	path_t *path = path_create("./");
	ASSERT_EQm("path_join(path, NULL) returns NULL", path_join(path, NULL), path);
	ASSERT_EQm("path_join(NULL, \"..\")", path_join(NULL, ".."), NULL);
	path_dispose(path);
	PASS();
}

TEST path_join_should_concatenate_two_paths()
{
	path_t *path = path_create(".");
	path_join(path, "..");
#ifdef __DOS__
	ASSERT_EQm("path '.' and '..' should be joined to '.\\..'", strcmp(path->str, ".\\.."), 0);
#else
	ASSERT_EQm("path '.' and '..' should be joined to './..'", strcmp(path->str, "./.."), 0);
#endif
	path_dispose(path);
	PASS();
}

TEST path_join_should_concatenate_two_paths_and_strip_a_duplicate_slash()
{
	path_t *path = path_create("./");
	path_join(path, "/..");
#ifdef __DOS__
	ASSERT_EQm("path '.' and '..' should be joined to '.\\..'", strcmp(path->str, ".\\.."), 0);
#else
	ASSERT_EQm("path '.' and '..' should be joined to './..'", strcmp(path->str, "./.."), 0);
#endif
	path_dispose(path);
	PASS();
}

SUITE(pathutil_tests)
{
	RUN_TEST(path_join_should_not_crash_when_any_param_is_null);
	RUN_TEST(path_join_should_concatenate_two_paths);
	RUN_TEST(path_join_should_concatenate_two_paths_and_strip_a_duplicate_slash);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(pathutil_tests);
    GREATEST_MAIN_END();        /* display results */
}
