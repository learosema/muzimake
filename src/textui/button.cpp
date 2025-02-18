#include <string.h>
#include "button.h"
#include "textmode.h"
#include "macros.h"

void button_render(ui_button_t *button)
{
	uint8_t color = button->color;
	if (button->active) {
		color = color & 0x0f;
	}

	if (button->focused) {
		textmode_dblrect(
			button->bounding_rect.x,
			button->bounding_rect.y,
			button->bounding_rect.width,
			button->bounding_rect.height,
			color
		);
	} else {
		textmode_rect(
			button->bounding_rect.x,
			button->bounding_rect.y,
			button->bounding_rect.width,
			button->bounding_rect.height,
			color
		);
	}
	if (button->bounding_rect.width <= 2 || button->bounding_rect.height <= 2) {
		return;
	}

	int len = MIN(strlen(button->label), button->bounding_rect.width-2);
	uint8_t centerX = (uint8_t)(button->bounding_rect.x + button->bounding_rect.width/2);
	uint8_t centerY = (uint8_t)(button->bounding_rect.y + button->bounding_rect.height/2);
	uint8_t labelX = (uint8_t)(centerX - len/2);
	uint8_t innerX = button->bounding_rect.x + 1;
	uint8_t innerY = button->bounding_rect.y + 1;
	uint8_t innerWidth = button->bounding_rect.width - 2;
	uint8_t innerHeight = button->bounding_rect.height - 2;
	for (uint8_t j = 0; j < innerHeight; j++) {
		uint8_t currentY = innerY + j;
		if (currentY != centerY) {
			textmode_hline(innerX, currentY, innerWidth, ' ', color);
		} else {
			uint8_t leftWidth = labelX - innerX;
			textmode_hline(innerX, centerY, leftWidth, ' ', color);
			textmode_hline(labelX + len, centerY, innerWidth - len - leftWidth, ' ', color);
			textmode_print(button->label, labelX, centerY, color);
		}
	}
}

ui_button_t button_create(char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
	ui_button_t button = {0};
	rect_t rect = {0};
	button.label = label;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	button.bounding_rect = rect;
	button.color = color;
	return button;
}

bool button_test_point_in_bounds(ui_button_t * button, uint8_t x, uint8_t y)
{
	uint8_t x0 = button->bounding_rect.x;
	uint8_t x1 = x0 + button->bounding_rect.width - 1;
	uint8_t y0 = button->bounding_rect.y;
	uint8_t y1 = y0 + button->bounding_rect.height - 1;
	return (
		x >= x0 && x <= x1 && y >= y0 && y <= y1
	);
}

bool button_test_mouse(ui_button_t * button, uint16_t mouseX, uint16_t mouseY)
{
	return button_test_point_in_bounds(button, (uint8_t)mouseX/8, (uint8_t)mouseY/8);
}
