#include <stdio.h>
#include <textmode.h>
#include <greatest.h>

#include "textui/cmponent.h"
#include "vendor/cp437.h"

static ui_component_t button;
const char *label = "OK";
const uint8_t button_id = 1;
const uint8_t button_x = 0;
const uint8_t button_y = 0;
const uint8_t button_width = 10;
const uint8_t button_height = 3;
const uint8_t button_color = 0x2f;

static int click_count = 0;
static bool spy_callback(uint16_t id, ui_event_t *event)
{
	if (event->type == UI_EVENT_CLICK) {
		click_count++;
	} 
	return true;
}

static void before_each(void *data) {
  textmode_init_headless(3);

	button = component_create_button(
		button_id,
		label,
		button_x, button_y,
		button_width, button_height,
		button_color
	);
	click_count = 0;
	
	button.component.button.event_handler = spy_callback;
}

static void after_each(void *data) {
	textmode_dispose();
}

TEST button_should_render()
{

	component_render(&button);
	uint8_t color = TEXT_GET_COLOR(button_x, button_y);

	ASSERT_EQ(button_color, color);
	ASSERT_EQm("Screen should have a box at the given coords", true, textmode_check_box(button_x, button_y, button_width, button_height));

	PASS();
}


TEST button_should_render_thick_when_focused()
{
	button.component.button.focused = true;
	component_render(&button);

	uint8_t color = TEXT_GET_COLOR(button_x, button_y);

	ASSERT_EQ(button_color, color);
	ASSERT_EQm("Screen should have a box at the given coords", true, textmode_check_dblbox(button_x, button_y, button_width, button_height));

	PASS();
}

TEST button_should_render_black_when_active()
{
	button.component.button.active = true;
	component_render(&button);

	uint8_t color = TEXT_GET_COLOR(button_x, button_y);

	ASSERT_EQ(button_color & 0xf, color);

	PASS();
}

TEST button_should_handle_space()
{
	ui_event_t event;
	event.type = UI_EVENT_KEY;
	event.payload.keyboard.keyCode = KEY_SPACE;

	button.component.generic.focused = false;
	button_process_events(&(button.component.button), &event);
	ASSERT_EQm("button should not trigger click on hitting space when not focused", click_count, 0);
	
	button.component.generic.focused = true;
	button_process_events(&(button.component.button), &event);
	ASSERT_EQm("button should trigger click on hitting space when focused", click_count, 1);

	PASS();
}

TEST button_should_handle_enter()
{
	ui_event_t event;
	event.type = UI_EVENT_KEY;
	event.payload.keyboard.keyCode = KEY_ENTER;

	button.component.generic.focused = false;
	button_process_events(&(button.component.button), &event);
	ASSERT_EQm("button should not trigger click on hitting enter when not focused", click_count, 0);
	
	button.component.generic.focused = true;
	button_process_events(&(button.component.button), &event);
	ASSERT_EQm("button should trigger click on hitting enter when focused", click_count, 1);
	
	PASS();
}

TEST button_should_handle_mousedown()
{
	ui_event_t event;
	event.type = UI_EVENT_MOUSEDOWN;
	event.payload.mouse.buttons = UI_EVENT_MOUSEDOWN;

	// click outside
	event.payload.mouse.x = button_x + button_width;
	event.payload.mouse.y = button_y + button_height;

	button.component.generic.focused = false;

	button_process_events(&(button.component.button), &event);
	ASSERT_EQm("button should not trigger click on clicking outside", click_count, 0);
	ASSERT_EQm("button should not receive focus on clicking outside", button.component.generic.focused, false);

	event.payload.mouse.x = button_x;
	event.payload.mouse.y = button_y;

	button_process_events(&(button.component.button), &event);
	ASSERT_EQm("button should trigger click on clicking inside", click_count, 1);
	ASSERT_EQm("button should receive focus on clicking inside", button.component.generic.focused, true);

	PASS();
}

SUITE(button_tests)
{
	SET_SETUP(before_each, nullptr);
	SET_TEARDOWN(after_each, nullptr);

	RUN_TEST(button_should_render);
	RUN_TEST(button_should_render_thick_when_focused);
	RUN_TEST(button_should_render_black_when_active);
	RUN_TEST(button_should_handle_space);
	RUN_TEST(button_should_handle_enter);
	RUN_TEST(button_should_handle_mousedown);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(button_tests);
    GREATEST_MAIN_END();        /* display results */
}
