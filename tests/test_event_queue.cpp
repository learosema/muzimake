#include "greatest.h"
#include "mainui.hpp"

TEST t_event_queue_single_click(void) {
    MainUI ui;
    GREATEST_ASSERT(ui.Initialize());

    /* enqueue a click on the PLAY button (local id = ID_PLAY = 2) */
    GREATEST_ASSERT(ui.DebugEnqueueClick(2));

    /* process queued events */
    ui.DebugProcessQueuedEvents();

    /* nothing should crash; UI should still be running (not done) */
    GREATEST_ASSERT_FALSE(ui.IsDone());

    ui.Shutdown();
    GREATEST_PASS();
}

TEST t_event_queue_multiple_clicks(void) {
    MainUI ui;
    GREATEST_ASSERT(ui.Initialize());

     /* enqueue several clicks across different local ids (avoid ID_LOAD=0
         because it triggers a modal in the real UI). */
     GREATEST_ASSERT(ui.DebugEnqueueClick(1));
     GREATEST_ASSERT(ui.DebugEnqueueClick(2));
     GREATEST_ASSERT(ui.DebugEnqueueClick(3));
     GREATEST_ASSERT(ui.DebugEnqueueClick(5));

    ui.DebugProcessQueuedEvents();

    GREATEST_ASSERT_FALSE(ui.IsDone());

    ui.Shutdown();
    GREATEST_PASS();
}

SUITE(event_queue_tests) {
    RUN_TEST(t_event_queue_single_click);
    RUN_TEST(t_event_queue_multiple_clicks);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(event_queue_tests);
    GREATEST_MAIN_END();
}
