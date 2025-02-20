#include <string.h>
#include "component.h"
#include "textmode.h"
#include "macros.h"

void input_render(ui_input_t *input)
{
	uint8_t input_x;
	uint8_t input_y;
	uint8_t input_len;
	size_t value_len;
	uint8_t i;
	uint8_t color = input->color;

	if (input->active || input->focused) {
		color = color & 0x0f;
	}
	if (input->bounding_rect.height != 3 && input->bounding_rect.height != 1) {
		input->bounding_rect.height = 1;
	}

	if (input->bounding_rect.height == 3)
	{
		if (input->focused) {
			textmode_dblrect(
				input->bounding_rect.x,
				input->bounding_rect.y,
				input->bounding_rect.width,
				input->bounding_rect.height,
				color
			);
		} else {
			textmode_rect(
				input->bounding_rect.x,
				input->bounding_rect.y,
				input->bounding_rect.width,
				input->bounding_rect.height,
				color
			);
			input_x = input->bounding_rect.x + 1;
			input_y = input->bounding_rect.y + 1;
			input_len = input->bounding_rect.width - 2;
		}
	}

	if (input->bounding_rect.height == 1)
	{
		input_x = input->bounding_rect.x;
		input_y = input->bounding_rect.y;
		input_len = input->bounding_rect.width;
	}

	value_len = strlen(input->value);

	for (i = 0; i < input_len; i++)
	{
		size_t offs = input->cursor_x0 + i;
		char ch = offs < value_len ? input->value[offs] : '.';
		textmode_putchar(ch, input_x, input_y, color);
	}

	if (input->focused) {
		textmode_colorize_area(input_x + input->cursor_x, input_y, 1, 1, INPUT_CURSOR_COLOR);
	}
}

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

bool rect_test_point_in_bounds(rect_t * rect, uint8_t x, uint8_t y)
{
	uint8_t x0 = rect->x;
	uint8_t x1 = x0 + rect->width - 1;
	uint8_t y0 = rect->y;
	uint8_t y1 = y0 + rect->height - 1;
	return (
		x >= x0 && x <= x1 && y >= y0 && y <= y1
	);
}

bool button_test_point_in_bounds(ui_button_t * button, uint8_t x, uint8_t y)
{
	return rect_test_point_in_bounds(&(button->bounding_rect), x, y);
}

bool button_test_mouse(ui_button_t * button, uint16_t mouseX, uint16_t mouseY)
{
	return button_test_point_in_bounds(button, (uint8_t)mouseX/8, (uint8_t)mouseY/8);
}

void button_process_events(uint8_t buttonCount, ui_button_t *buttons, ui_event_t *event)
{
	for (uint8_t i = 0; i < buttonCount; i++)
	{
		ui_button_t * button = &(buttons[i]);
		switch (event->type) {
			case UI_EVENT_MOUSEMOVE:
				if ((button->active)||
						(button_test_mouse(button, event->payload.mouse.x, event->payload.mouse.y)))
				{
					button_render(button);
				}
				break;
			case UI_EVENT_MOUSEDOWN:
				if (button_test_mouse(button, event->payload.mouse.x, event->payload.mouse.y)) {
					button->active = true;
					button_render(button);
				}
				break;
			case UI_EVENT_MOUSEUP:
				if (button->active) {
					button->active = false;
					button_render(button);
					if (button_test_mouse(button, event->payload.mouse.x, event->payload.mouse.y)) {
						// emitEvent('click', button);
					}
				}
				break;
			case UI_EVENT_KEY:
				// todo: shortcuts?
				break;
			default:
				break;
		}
	}
}



void component_render(ui_component_t *component)
{
	if (component == nullptr) {
		return;
	}
	switch (component->type)
	{
		case COMPONENT_BUTTON:
			button_render(&(component->component.button));
			break;
		case COMPONENT_INPUT:
			input_render(&(component->component.input));
			break;
	}
}
