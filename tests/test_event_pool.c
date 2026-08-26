#include <greatest.h>

#include "textui/ui_event.h"

TEST mouse_move_should_reuse_event(void)
{
    ui_event_pool_t pool;
    ui_event_pool_init(&pool, 4);
    ui_event_begin(&pool);

    ui_event_mouse_move(&pool, 10, 20, 1, 2);
    ui_event_mouse_move(&pool, 15, 25, 3, 4);

    ASSERT_EQm("should have only one event", 1u, pool.size);
    ASSERT_EQm("x should be latest", 15u, pool.events[0].payload.mouse.x);
    ASSERT_EQm("y should be latest", 25u, pool.events[0].payload.mouse.y);
    ASSERT_EQm("deltaX should be sum", 4, pool.events[0].payload.mouse.deltaX);
    ASSERT_EQm("deltaY should be sum", 6, pool.events[0].payload.mouse.deltaY);

    ui_event_pool_dispose(&pool);
    PASS();
}

SUITE(event_pool_tests)
{
    RUN_TEST(mouse_move_should_reuse_event);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(event_pool_tests);
    GREATEST_MAIN_END();
}
