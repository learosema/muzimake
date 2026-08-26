#include "greatest.h"
#include "pathutil.h"
#include <string.h>

/* ---------- path_create ---------- */

TEST create_should_copy_string(void) {
	path_t *p = path_create("foo/bar");
	ASSERT(p != NULL);
	ASSERT_STR_EQ("foo/bar", p->str);
	ASSERT_EQ(strlen("foo/bar"), p->len);
	path_dispose(p);
	PASS();
}

TEST create_should_handle_empty_string(void) {
	path_t *p = path_create("");
	ASSERT(p != NULL);
	ASSERT_EQ(0, p->len);
	ASSERT_STR_EQ("", p->str);
	path_dispose(p);
	PASS();
}

TEST create_should_return_null_on_null_input(void) {
	path_t *p = path_create(NULL);
	ASSERT_EQ(NULL, p);
	PASS();
}

/* ---------- path_join ---------- */

TEST join_should_add_separator_between_segments(void) {
	path_t *p = path_create("foo");
	p = path_join(p, "bar");
	ASSERT_STR_EQ("foo" "/" "bar", p->str); /* PATH_SEP je nach Plattform */
	path_dispose(p);
	PASS();
}

TEST join_should_not_duplicate_separator_when_first_ends_with_sep(void) {
	path_t *p = path_create("foo/");
	p = path_join(p, "bar");
	ASSERT_STR_EQ("foo/bar", p->str);
	path_dispose(p);
	PASS();
}

TEST join_should_not_duplicate_separator_when_second_starts_with_sep(void) {
	path_t *p = path_create("foo");
	p = path_join(p, "/bar");
	ASSERT_STR_EQ("foo/bar", p->str);
	path_dispose(p);
	PASS();
}

TEST join_should_normalize_other_separator(void) {
	path_t *p = path_create("foo");
	p = path_join(p, "bar\\baz");
	/* OTHER_PATH_SEP sollte überall zu PATH_SEP werden */
	ASSERT(strchr(p->str, OTHER_PATH_SEP) == NULL);
	path_dispose(p);
	PASS();
}

TEST join_should_return_original_path_on_null_arguments(void) {
	path_t *p = path_create("foo");
	path_t *result = path_join(p, NULL);
	ASSERT_EQ(p, result);
	path_dispose(p);

	result = path_join(NULL, "bar");
	ASSERT_EQ(NULL, result);
	PASS();
}

TEST join_should_update_len_correctly(void) {
	path_t *p = path_create("foo");
	p = path_join(p, "bar");
	ASSERT_EQ(strlen(p->str), p->len);
	path_dispose(p);
	PASS();
}

/* ---------- path_chdir ---------- */

TEST chdir_should_succeed_for_existing_directory(void) {
	/* Annahme: "." existiert immer, plattformunabhängig sicher */
	ASSERT(path_chdir(".") == true);
	PASS();
}

TEST chdir_should_fail_for_nonexistent_directory(void) {
	ASSERT(path_chdir("this_directory_should_not_exist_12345") == false);
	PASS();
}

/* ---------- Suite ---------- */

SUITE(path_create_suite) {
	RUN_TEST(create_should_copy_string);
	RUN_TEST(create_should_handle_empty_string);
	RUN_TEST(create_should_return_null_on_null_input);
}

SUITE(path_join_suite) {
	RUN_TEST(join_should_add_separator_between_segments);
	RUN_TEST(join_should_not_duplicate_separator_when_first_ends_with_sep);
	RUN_TEST(join_should_not_duplicate_separator_when_second_starts_with_sep);
	RUN_TEST(join_should_normalize_other_separator);
	RUN_TEST(join_should_return_original_path_on_null_arguments);
	RUN_TEST(join_should_update_len_correctly);
}

SUITE(path_chdir_suite) {
	RUN_TEST(chdir_should_succeed_for_existing_directory);
	RUN_TEST(chdir_should_fail_for_nonexistent_directory);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(path_create_suite);
	RUN_SUITE(path_join_suite);
	RUN_SUITE(path_chdir_suite);
	GREATEST_MAIN_END();
}

