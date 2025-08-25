#include <stdio.h>
#include <textmode.h>
#include <greatest.h>

#include "textui/cmponent.h"
#include "vendor/cp437.h"

static void before_each(void *data) {
  textmode_init_headless(3);
}

static void after_each(void *data) {
	textmode_dispose();
}

#define GET_CHAR(info, x, y) (uint8_t)(*(info->vram + 2 * (info->numCols * (y) + (x))))
#define GET_COLOR(info, x, y) (uint8_t)(*(info->vram + 1 + 2 * (info->numCols * (y) + (x))))

TEST button_should_render()
{
	const char *label = "OK";
	const uint8_t button_id = 1;
	const uint8_t button_x = 0;
	const uint8_t button_y = 0;
	const uint8_t button_width = 10;
	const uint8_t button_height = 3;
	const uint8_t button_color = 0x2f;

	ui_component_t button = component_create_button(
		button_id,
		label,
		button_x, button_y,
		button_width, button_height,
		button_color
	);
	component_render(&button);


	MODEINFO* info = textmode_get_modeinfo();

	uint8_t color = GET_COLOR(info, 0, 0);
	char topleft_corner = GET_CHAR(info, 0, 0);
	char topright_corner = GET_CHAR(info, button_width - 1, 0);
	char btmleft_corner = GET_CHAR(info, 0, button_height - 1);
	char btmright_corner = GET_CHAR(info, button_width - 1, button_height - 1);

	ASSERT_EQ(button_color, color);
	ASSERT_EQ(CP_THIN_RIGHT_THIN_DOWN, topleft_corner);
	ASSERT_EQ(CP_THIN_LEFT_THIN_DOWN, topright_corner);
	ASSERT_EQ(CP_THIN_RIGHT_THIN_UP, btmleft_corner);
	ASSERT_EQ(CP_THIN_LEFT_THIN_UP, btmright_corner);
	for (uint8_t x = 1; x < button_width - 2; x++) {
		ASSERT_EQ(CP_THIN_HORIZONTAL, GET_CHAR(info, x, 0));
		ASSERT_EQ(CP_THIN_HORIZONTAL, GET_CHAR(info, x, button_height - 1));
	}
	for (uint8_t y = 1; y < button_height - 2; y++) {
		ASSERT_EQ(CP_THIN_HORIZONTAL, GET_CHAR(info, 0, y));
		ASSERT_EQ(CP_THIN_HORIZONTAL, GET_CHAR(info, button_width - 1, y));
	}

	PASS();
}

SUITE(button_tests)
{
	SET_SETUP(before_each, nullptr);
	SET_TEARDOWN(after_each, nullptr);

	RUN_TEST(button_should_render);
}



GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* init & parse command-line args */
    RUN_SUITE(button_tests);
    GREATEST_MAIN_END();        /* display results */
}
