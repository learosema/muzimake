/* Edge-case tests for the MainUI event queue */
#include "greatest.h"
#include "mainui.hpp"

TEST t_event_queue_process_empty_queue(void) {
    MainUI ui;
    GREATEST_ASSERT(ui.Initialize());

    /* process when queue is empty: should be a no-op and not crash */
    ui.DebugProcessQueuedEvents();

    GREATEST_ASSERT_FALSE(ui.IsDone());

    ui.Shutdown();
    GREATEST_PASS();
}

TEST t_event_queue_enqueue_invalid_id_returns_false(void) {
    MainUI ui;
    GREATEST_ASSERT(ui.Initialize());

    /* use an out-of-range local id - should return false */
    GREATEST_ASSERT_FALSE(ui.DebugEnqueueClick(255));

    ui.Shutdown();
    GREATEST_PASS();
}

SUITE(event_queue_edge_tests) {
    RUN_TEST(t_event_queue_process_empty_queue);
    RUN_TEST(t_event_queue_enqueue_invalid_id_returns_false);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(event_queue_edge_tests);
    GREATEST_MAIN_END();
}
