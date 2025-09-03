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


static void before_each(void *data) {
  textmode_init_headless(3);

	button = component_create_button(
		button_id,
		label,
		button_x, button_y,
		button_width, button_height,
		button_color
	);
}

static void after_each(void *data) {
	textmode_dispose();
}

#define GET_CHAR(info, x, y) (uint8_t) \
	((((x) < 0) || ((x) >= info->numCols) || ((y) < 0) || ((y) >= info->numRows)) ? 0 : \
	*(info->vram + 2 * (info->numCols * (y) + (x))))

#define GET_COLOR(info, x, y) (uint8_t) \
	((((x) < 0) || ((x) >= info->numCols) || ((y) < 0) || ((y) >= info->numRows)) ? 0 : \
	*(info->vram + 1 + 2 * (info->numCols * (y) + (x))))

bool check_thin_box(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	MODEINFO* info = textmode_get_modeinfo();
	char topleft_corner = GET_CHAR(info, x, y);
	char topright_corner = GET_CHAR(info, x + width - 1, 0);
	char btmleft_corner = GET_CHAR(info, y, y + height - 1);
	char btmright_corner = GET_CHAR(info, x + width - 1, y + height - 1);
	if (CP_THIN_RIGHT_THIN_DOWN != topleft_corner) return false;
	if (CP_THIN_LEFT_THIN_DOWN != topright_corner) return false;
	if (CP_THIN_RIGHT_THIN_UP != btmleft_corner) return false;
	if (CP_THIN_LEFT_THIN_UP != btmright_corner) return false;
	for (uint8_t i = 1; i < width - 2; i++) {
		if (CP_THIN_HORIZONTAL != GET_CHAR(info, x + i, y)) return false;
		if (CP_THIN_HORIZONTAL != GET_CHAR(info, x + i, y + height - 1)) return false;
	}
	for (uint8_t i = 1; i < height - 2; i++) {
		if (CP_THIN_HORIZONTAL != GET_CHAR(info, x, y + i)) return false;
		if (CP_THIN_HORIZONTAL != GET_CHAR(info, x + width - 1, y + i)) return false;
	}
	return true;
}

bool check_thick_box(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	MODEINFO* info = textmode_get_modeinfo();
	char topleft_corner = GET_CHAR(info, x, y);
	char topright_corner = GET_CHAR(info, x + width - 1, 0);
	char btmleft_corner = GET_CHAR(info, y, y + height - 1);
	char btmright_corner = GET_CHAR(info, x + width - 1, y + height - 1);
	if (CP_THICK_RIGHT_THICK_DOWN != topleft_corner) return false;
	if (CP_THICK_LEFT_THICK_DOWN != topright_corner) return false;
	if (CP_THICK_RIGHT_THICK_UP != btmleft_corner) return false;
	if (CP_THICK_LEFT_THICK_UP != btmright_corner) return false;
	for (uint8_t i = 1; i < width - 2; i++) {
		if (CP_THICK_HORIZONTAL != GET_CHAR(info, x + i, y)) return false;
		if (CP_THICK_HORIZONTAL != GET_CHAR(info, x + i, y + height - 1)) return false;
	}
	for (uint8_t i = 1; i < height - 2; i++) {
		if (CP_THICK_HORIZONTAL != GET_CHAR(info, x, y + i)) return false;
		if (CP_THICK_HORIZONTAL != GET_CHAR(info, x + width - 1, y + i)) return false;
	}
	return true;
}


TEST button_should_render()
{

	component_render(&button);
	MODEINFO* info = textmode_get_modeinfo();
	uint8_t color = GET_COLOR(info, button_x, button_y);

	ASSERT_EQ(button_color, color);
	ASSERT_EQm("Screen should have a box at the given coords", true, check_thin_box(button_x, button_y, button_width, button_height));

	PASS();
}


TEST button_should_render_thick_when_focused()
{
	button.component.button.focused = true;
	component_render(&button);
	MODEINFO* info = textmode_get_modeinfo();
	uint8_t color = GET_COLOR(info, button_x, button_y);

	ASSERT_EQ(button_color, color);
	ASSERT_EQm("Screen should have a box at the given coords", true, check_thick_box(button_x, button_y, button_width, button_height));

	PASS();
}

TEST button_should_render_black_when_active()
{
	button.component.button.active = true;
	component_render(&button);
	MODEINFO* info = textmode_get_modeinfo();
	uint8_t color = GET_COLOR(info, button_x, button_y);

	ASSERT_EQ(button_color & 0xf, color);
	PASS();
}

SUITE(button_tests)
{
	SET_SETUP(before_each, nullptr);
	SET_TEARDOWN(after_each, nullptr);

	RUN_TEST(button_should_render);
	RUN_TEST(button_should_render_thick_when_focused);
	RUN_TEST(button_should_render_black_when_active);
}



GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(button_tests);
    GREATEST_MAIN_END();        /* display results */
}
