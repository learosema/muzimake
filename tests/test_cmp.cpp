
#include "greatest.h"
#include <assert.h>

int add(int a, int b) {
	return a + b;
}


TEST foo_should_foo() {
  PASS();
}

TEST add_should_add() {
	assert(add(3,4) == 7);
	PASS();
}

static void setup_cb(void *data) {
    printf("setup callback for each test case\n");
}

static void teardown_cb(void *data) {
    printf("teardown callback for each test case\n");
}

SUITE(suite) {
    /* Optional setup/teardown callbacks which will be run before/after
     * every test case. If using a test suite, they will be cleared when
     * the suite finishes. */
    SET_SETUP(setup_cb, nullptr);
    SET_TEARDOWN(teardown_cb, nullptr);

    RUN_TEST(foo_should_foo);
		RUN_TEST(add_should_add);
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

/* Set up, run suite(s) of tests, report pass/fail/skip stats. */
int run_tests(void) {
    GREATEST_INIT();            /* init. greatest internals */
    /* List of suites to run (if any). */
    RUN_SUITE(suite);

    /* Tests can also be run directly, without using test suites. */
    RUN_TEST(foo_should_foo);

    GREATEST_PRINT_REPORT();          /* display results */
    return greatest_all_passed();
}

/* main(), for a standalone command-line test runner.
 * This replaces run_tests above, and adds command line option
 * handling and exiting with a pass/fail status. */
int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(suite);
    GREATEST_MAIN_END();        /* display results */
}
