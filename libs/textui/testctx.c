#include "greatest.h"
#include "ui_ctx.h"

#define RECT(_x, _y, _w, _h) ((rect_t){ .x = (_x), .y = (_y), .width = (_w), .height = (_h) })

/* --- Annahme (2): Button-IDs --- */
#define BTN_LEFT  0
#define BTN_RIGHT 1

/* ---------- init ---------- */

TEST init_gives_sane_defaults(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ASSERT_EQ(0, ctx.input.mouse.x);
    ASSERT_EQ(0, ctx.input.mouse.y);
    ASSERT_EQ(0, ctx.input.mouse.buttons);
    ASSERT_EQ(0, ctx.input.mouse.last_event);
    ASSERT_EQ(0, ctx.input.keyboard.text_length);

    for (int i = 0; i < UI_KEY_MAX; i++) {
        ASSERT_FALSE(ctx.input.keyboard.keys[i]);
    }

    PASS();
}

/* ---------- Maus: Bewegung ---------- */

TEST mouse_motion_updates_position_and_delta(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_mouse(&ctx, UI_EVENT_MOUSEMOVE, 42, 17, 5, -3, 0);
    ui_context_end_frame(&ctx);

    ASSERT_EQ(42, ctx.input.mouse.x);
    ASSERT_EQ(17, ctx.input.mouse.y);
    ASSERT_EQ(5, ctx.input.mouse.deltaX);
    ASSERT_EQ(-3, ctx.input.mouse.deltaY);
    ASSERT(ui_context_is_mouse_moved(&ctx));

    PASS();
}

TEST mouse_not_moved_flag_clears_next_frame(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_mouse(&ctx, UI_EVENT_MOUSEMOVE, 10, 10, 1, 1, 0);
    ui_context_end_frame(&ctx);
    ASSERT(ui_context_is_mouse_moved(&ctx));

    /* Neuer Frame ohne neues Motion-Event -> Flag muss weg sein */
    ui_context_begin_frame(&ctx);
    ui_context_end_frame(&ctx);
    ASSERT_FALSE(ui_context_is_mouse_moved(&ctx));

    PASS();
}

/* ---------- Maus: Klicks (Edge-Detection) ---------- */

TEST mouse_click_is_edge_triggered_press(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_click(&ctx, UI_EVENT_MOUSEDOWN, 1 /* buttons mask, linke Taste */);
    ui_context_end_frame(&ctx);

    ASSERT(ui_context_is_mouse_pressed(&ctx, BTN_LEFT));
    ASSERT_FALSE(ui_context_is_mouse_released(&ctx, BTN_LEFT));

    PASS();
}

TEST mouse_press_flag_does_not_persist_across_frames(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_click(&ctx, UI_EVENT_MOUSEDOWN, 1);
    ui_context_end_frame(&ctx);
    ASSERT(ui_context_is_mouse_pressed(&ctx, BTN_LEFT));

    /* Taste bleibt "gedrückt gehalten", aber es kommt kein neues DOWN-Event
     * mehr -> is_mouse_pressed() (Edge) muss false sein, auch wenn die
     * Taste laut buttons-Bitmask noch aktiv ist. */
    ui_context_begin_frame(&ctx);
    ui_context_end_frame(&ctx);
    ASSERT_FALSE(ui_context_is_mouse_pressed(&ctx, BTN_LEFT));

    PASS();
}

TEST mouse_click_press_then_release_cycle(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_click(&ctx, UI_EVENT_MOUSEDOWN, 1);
    ui_context_end_frame(&ctx);
    ASSERT(ui_context_is_mouse_pressed(&ctx, BTN_LEFT));

    ui_context_begin_frame(&ctx);
    ui_context_input_click(&ctx, UI_EVENT_MOUSEUP, 0);
    ui_context_end_frame(&ctx);
    ASSERT(ui_context_is_mouse_released(&ctx, BTN_LEFT));
    ASSERT_FALSE(ui_context_is_mouse_pressed(&ctx, BTN_LEFT));

    PASS();
}

TEST mouse_left_and_right_button_are_independent(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_click(&ctx, UI_EVENT_MOUSEDOWN_R, 1);
    ui_context_end_frame(&ctx);

    ASSERT(ui_context_is_mouse_pressed(&ctx, BTN_RIGHT));
    ASSERT_FALSE(ui_context_is_mouse_pressed(&ctx, BTN_LEFT));

    PASS();
}

/* ---------- Hit-Testing ---------- */

TEST mouse_inside_rect_detects_hit(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);
    rect_t r = RECT(10, 10, 20, 20); /* x,y,w,h -> deckt 10..30 in beiden Achsen ab */

    ui_context_begin_frame(&ctx);
    ui_context_input_mouse(&ctx, UI_EVENT_MOUSEMOVE, 15, 15, 0, 0, 0);
    ui_context_end_frame(&ctx);

    ASSERT(ui_context_is_mouse_inside_rect(&ctx, &r));

    PASS();
}

TEST mouse_outside_rect_is_not_a_hit(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);
    rect_t r = RECT(10, 10, 20, 20);

    ui_context_begin_frame(&ctx);
    ui_context_input_mouse(&ctx, UI_EVENT_MOUSEMOVE, 100, 100, 0, 0, 0);
    ui_context_end_frame(&ctx);

    ASSERT_FALSE(ui_context_is_mouse_inside_rect(&ctx, &r));

    PASS();
}

TEST mouse_on_rect_edge_is_a_hit(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);
    rect_t r = RECT(10, 10, 20, 20);

    ui_context_begin_frame(&ctx);
    ui_context_input_mouse(&ctx, UI_EVENT_MOUSEMOVE, 10, 10, 0, 0, 0);
    ui_context_end_frame(&ctx);

    ASSERT(ui_context_is_mouse_inside_rect(&ctx, &r));

    PASS();
}

TEST is_mouse_inside_rect_matches_rect_test_mouse_directly(void) {
    /* Cross-Check: ui_context_is_mouse_inside_rect() sollte sich exakt wie
     * rect_test_mouse() aus geometry.h verhalten, nur eben gespeist aus der
     * aktuellen ctx-Mausposition statt aus frei übergebenen Koordinaten. */
    ui_context_t ctx;
    ui_context_init(&ctx);
    rect_t r = RECT(50, 50, 30, 10);

    ui_context_begin_frame(&ctx);
    ui_context_input_mouse(&ctx, UI_EVENT_MOUSEMOVE, 60, 55, 0, 0, 0);
    ui_context_end_frame(&ctx);

    bool expected = rect_test_mouse(&r, ctx.input.mouse.x, ctx.input.mouse.y);
    ASSERT_EQ(expected, ui_context_is_mouse_inside_rect(&ctx, &r));

    PASS();
}

/* ---------- Tastatur: Sondertasten (Edge-Detection) ---------- */

TEST key_press_is_edge_triggered(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_key(&ctx, UI_KEY_ENTER, true);
    ui_context_end_frame(&ctx);

    ASSERT(ui_context_is_key_pressed(&ctx, UI_KEY_ENTER));
    ASSERT_FALSE(ui_context_is_key_released(&ctx, UI_KEY_ENTER));

    PASS();
}

TEST key_release_after_press(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_key(&ctx, UI_KEY_TAB, true);
    ui_context_end_frame(&ctx);
    ASSERT(ui_context_is_key_pressed(&ctx, UI_KEY_TAB));

    ui_context_begin_frame(&ctx);
    ui_context_input_key(&ctx, UI_KEY_TAB, false);
    ui_context_end_frame(&ctx);
    ASSERT(ui_context_is_key_released(&ctx, UI_KEY_TAB));
    ASSERT_FALSE(ui_context_is_key_pressed(&ctx, UI_KEY_TAB));

    PASS();
}

TEST held_key_does_not_repeatedly_report_pressed(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_key(&ctx, UI_KEY_LEFT, true);
    ui_context_end_frame(&ctx);
    ASSERT(ui_context_is_key_pressed(&ctx, UI_KEY_LEFT));

    /* Taste bleibt physisch unten, aber kein neues Event diesen Frame */
    ui_context_begin_frame(&ctx);
    ui_context_end_frame(&ctx);
    ASSERT_FALSE(ui_context_is_key_pressed(&ctx, UI_KEY_LEFT));

    PASS();
}

TEST different_keys_do_not_interfere(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_key(&ctx, UI_KEY_UP, true);
    ui_context_end_frame(&ctx);

    ASSERT(ui_context_is_key_pressed(&ctx, UI_KEY_UP));
    ASSERT_FALSE(ui_context_is_key_pressed(&ctx, UI_KEY_DOWN));
    ASSERT_FALSE(ui_context_is_key_pressed(&ctx, UI_KEY_ENTER));

    PASS();
}

/* ---------- Text-Eingabe ---------- */

TEST char_input_appends_to_text_buffer(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_char(&ctx, 'H');
    ui_context_input_char(&ctx, 'i');
    ui_context_end_frame(&ctx);

    ASSERT_EQ(2, ctx.input.keyboard.text_length);
    ASSERT_EQ('H', ctx.input.keyboard.text[0]);
    ASSERT_EQ('i', ctx.input.keyboard.text[1]);

    PASS();
}

TEST char_input_buffer_resets_each_frame(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    ui_context_input_char(&ctx, 'X');
    ui_context_end_frame(&ctx);
    ASSERT_EQ(1, ctx.input.keyboard.text_length);

    /* Neuer Frame ohne neuen Char-Input -> Puffer muss leer sein,
     * sonst würde derselbe Tastendruck über mehrere Frames hinweg
     * wiederholt in ein Textfeld eingefügt. */
    ui_context_begin_frame(&ctx);
    ui_context_end_frame(&ctx);
    ASSERT_EQ(0, ctx.input.keyboard.text_length);

    PASS();
}

TEST char_input_does_not_overflow_buffer(void) {
    ui_context_t ctx;
    ui_context_init(&ctx);

    ui_context_begin_frame(&ctx);
    /* Absichtlich mehr Zeichen füttern, als UI_TEXT_INPUT_MAX erlaubt */
    for (int i = 0; i < UI_TEXT_INPUT_MAX + 8; i++) {
        ui_context_input_char(&ctx, 'a');
    }
    ui_context_end_frame(&ctx);

    ASSERT(ctx.input.keyboard.text_length <= UI_TEXT_INPUT_MAX);

    PASS();
}

/* ---------- Suite ---------- */

SUITE(ui_ctx_suite) {
    RUN_TEST(init_gives_sane_defaults);

    RUN_TEST(mouse_motion_updates_position_and_delta);
    RUN_TEST(mouse_not_moved_flag_clears_next_frame);

    RUN_TEST(mouse_click_is_edge_triggered_press);
    RUN_TEST(mouse_press_flag_does_not_persist_across_frames);
    RUN_TEST(mouse_click_press_then_release_cycle);
    RUN_TEST(mouse_left_and_right_button_are_independent);

    RUN_TEST(mouse_inside_rect_detects_hit);
    RUN_TEST(mouse_outside_rect_is_not_a_hit);
    RUN_TEST(mouse_on_rect_edge_is_a_hit);
    RUN_TEST(is_mouse_inside_rect_matches_rect_test_mouse_directly);

    RUN_TEST(key_press_is_edge_triggered);
    RUN_TEST(key_release_after_press);
    RUN_TEST(held_key_does_not_repeatedly_report_pressed);
    RUN_TEST(different_keys_do_not_interfere);

    RUN_TEST(char_input_appends_to_text_buffer);
    RUN_TEST(char_input_buffer_resets_each_frame);
    RUN_TEST(char_input_does_not_overflow_buffer);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(ui_ctx_suite);
    GREATEST_MAIN_END();
}
