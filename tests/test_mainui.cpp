#include "greatest.h"
#include "mainui.hpp"

TEST t_initialize_shutdown_states(void) {
    MainUI ui;
    GREATEST_ASSERT_FALSE(ui.IsDone());
    GREATEST_ASSERT(ui.Initialize());
    GREATEST_ASSERT_FALSE(ui.IsDone());
    ui.Shutdown();
    GREATEST_ASSERT(ui.IsDone());
    GREATEST_PASS();
}

TEST t_instance_scoped_ids(void) {
    MainUI a;
    MainUI b;
    GREATEST_ASSERT(a.Initialize());
    /* Second initialize should fail in singleton mode */
    GREATEST_ASSERT_FALSE(b.Initialize());

    unsigned id_a = a.DebugGetFirstComponentId();
    unsigned id_b = b.DebugGetFirstComponentId();

    /* first instance uses local ids; the first component is ID_LOAD == 0 */
    GREATEST_ASSERT_EQ(id_a, 0);
    GREATEST_ASSERT_EQ(id_b, 0);

    a.Shutdown();
    GREATEST_PASS();
}

SUITE(mainui_tests) {
    RUN_TEST(t_initialize_shutdown_states);
    RUN_TEST(t_instance_scoped_ids);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(mainui_tests);
    GREATEST_MAIN_END();
}
