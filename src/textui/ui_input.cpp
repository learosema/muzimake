#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "textmode.h"
#include "ui_input.h"

void input_render(ui_input_t *input)
{
	uint8_t input_x;
	uint8_t input_y;
	uint8_t input_len;
	size_t value_len;
	uint8_t i;
	uint8_t color = input->color;

	if (input->bounding_rect.height != 3 && input->bounding_rect.height != 1) {
		input->bounding_rect.height = 1;
	}

	if (input->bounding_rect.height == 3)
	{
		input_x = input->bounding_rect.x + 1;
		input_y = input->bounding_rect.y + 1;
		input_len = input->bounding_rect.width - 2;
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
		textmode_putchar_color(input_x + i, input_y, ch, color);
	}

	if (input->focused) {
		textmode_colorize_area(input_x + input->cursor_x, input_y, 1, 1, INPUT_CURSOR_COLOR);
	}
}

void input_process_events(ui_input_t *input, ui_event_t *event)
{
	if ((event->type & UI_EVENT_MOUSEUP) > 0) {
		ui_handle_mouseup((ui_generic_t *)input, event);
		return;
	}

	switch (event->type) {
		case UI_EVENT_KEY: {
			uint16_t input_width = input->bounding_rect.width;
			if (! input->focused) {
				break;
			}
			if (input->bounding_rect.height == 3) {
				input_width -= 2;
			}
			if (event->payload.keyboard.keyCode == KEY_ARROW_LEFT) {
				if (input->cursor_x > 0) {
					input->cursor_x -= 1;
				} else {
					if (input->cursor_x0 > 0) {
						input->cursor_x0 -= 1;
					}
				}
			}
			if (event->payload.keyboard.keyCode == KEY_ENTER) {
				input->focused = false;
				input->paint = true;
				return;
			}
			if (event->payload.keyboard.keyCode == KEY_ARROW_RIGHT) {
				if (input->cursor_x < input_width - 1) {
					input->cursor_x += 1;
				} else {
					if (input->cursor_x0 + input->cursor_x < input->maxlen) {
						input->cursor_x0 += 1;
					}
				}
			}
			if (event->payload.keyboard.keyCode == KEY_ALT_I)
			{
				input->overwrite = !(input->overwrite);
				input->paint = true;
			}
			if (event->payload.keyboard.keyCode == KEY_BACKSPACE)
			{
				uint16_t x = input->cursor_x0 + input->cursor_x;
				if (x > 0) {
					input->paint = true;
					input->value[x - 1] = input->value[x] == '\0' ? '\0' : ' ';
					if (input->cursor_x > 0) {
						input->cursor_x -= 1;
					} else {
						if (input->cursor_x0 > 0) {
							input->cursor_x0 -= 1;
						}
					}
				}
				return;
			}
			if (event->payload.keyboard.keyCode == 27) {
				input->focused = false;
				input->cursor_x = 0;
				input->cursor_x0 = 0;
				input->paint = true;
				return;
			}
			if (event->payload.keyboard.keyCode <= 255) {
				uint16_t x = input->cursor_x0 + input->cursor_x;
				if (x < input->maxlen) {
					input->value[x] = event->payload.keyboard.keyCode & 0xff;
					for (uint16_t c = x-1; c>=0; c--)
					{
						if (input->value[c] == '\0') {
							input->value[c] = ' ';
						} else {
							break;
						}
					}

					if (input->cursor_x < input_width - 1)
					{
						input->cursor_x++;
					} else {
						input->cursor_x0++;
					}
				}
			}
			input->paint = true;
			break;
		}
	}
}
