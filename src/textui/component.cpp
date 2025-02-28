#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "textmode.h"
#include "macros.h"
#include "vendor/cp437.h"

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

bool rect_test_mouse(rect_t * rect, uint16_t mouseX, uint16_t mouseY)
{
	return rect_test_point_in_bounds(rect, (uint8_t)(mouseX>>3), (uint8_t)(mouseY>>3));
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

void button_process_events(ui_button_t *button, ui_event_t *event)
{
	if (button->event_handler != nullptr) {
		bool result = button->event_handler(button->id, event);
		if (result == false) {
			return;
		}
	}
	switch (event->type) {
		case UI_EVENT_MOUSEMOVE:
			if ((button->active)&&
					(rect_test_mouse(&(button->bounding_rect), event->payload.mouse.x, event->payload.mouse.y)))
			{
				button->paint = true;
			}
			break;
		case UI_EVENT_MOUSEDOWN:
			if (rect_test_mouse(&(button->bounding_rect), event->payload.mouse.x, event->payload.mouse.y)) {
				button->active = true;
				button->paint = true;
			}
			break;
		case UI_EVENT_MOUSEUP:
			if (button->active) {
				button->active = false;
				button->paint = true;
				if (rect_test_mouse(&(button->bounding_rect), event->payload.mouse.x, event->payload.mouse.y)) {
					if (button->event_handler != nullptr) {
						ui_event_t click = {0};
						click.type = UI_EVENT_CLICK;
						click.payload.click.buttons = event->payload.mouse.buttons;
						click.payload.click.target = button->id;
						bool result = button->event_handler(button->id, &click);
						if (result == false) {
							return;
						}
					}
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
	switch (event->type) {
		case UI_EVENT_MOUSEUP: {
			ui_event_t clickEvent = {0};
			clickEvent.type = UI_EVENT_CLICK;
			clickEvent.payload.click.target = input->id;
			clickEvent.payload.click.buttons = event->payload.mouse.buttons;
			if (! rect_test_mouse(&(input->bounding_rect), event->payload.mouse.x, event->payload.mouse.y))
			{
				if (input->focused) {
					input->focused = false;
					input->paint = true;
				}
				return;
			}
			if (input->event_handler != nullptr) {
				bool result = input->event_handler(input->id, &clickEvent);
				if (! result) {
					break;
				}
			}
			input->focused = true;
			input->paint = true;
			break;
		}
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

void listbox_render(ui_listbox_t *listbox)
{
	if (listbox->focused) {
		textmode_dblrect(
			listbox->bounding_rect.x,
			listbox->bounding_rect.y,
			listbox->bounding_rect.width,
			listbox->bounding_rect.height,
			listbox->color
		);
	} else {
		textmode_rect(
			listbox->bounding_rect.x,
			listbox->bounding_rect.y,
			listbox->bounding_rect.width,
			listbox->bounding_rect.height,
			listbox->color
		);
	}
	uint8_t inner_width = listbox->bounding_rect.width - 2;
	uint8_t inner_height = listbox->bounding_rect.height - 2;
	uint8_t select_color = 0xf + (listbox->color & 0x70 ^ 0x70);
	for (uint8_t y = 0; y < inner_height; y++)
	{
		uint16_t yy = listbox->cursor_y0 + y;
		uint8_t color = yy == (listbox->cursor_y0 + listbox->cursor_y) ?
			select_color : listbox->color;
		if (yy < listbox->num_items) {
			uint8_t written = textmode_printn(listbox->values[yy], inner_width,
				listbox->bounding_rect.x + 1,
				listbox->bounding_rect.y + 1 + y, color
			);
			textmode_hline(
				listbox->bounding_rect.x + 1 + written,
				listbox->bounding_rect.y + 1 + y,
				inner_width - written, ' ', color
			);
		} else {
			textmode_hline(
				listbox->bounding_rect.x + 1,
				listbox->bounding_rect.y + 1 + y,
				inner_width, ' ', color
			);
		}
	}
}

void listbox_process_events(ui_listbox_t *listbox, ui_event_t *event)
{
	switch (event->type) {
		case UI_EVENT_MOUSEUP: {
			ui_event_t clickEvent = {0};
			clickEvent.type = UI_EVENT_CLICK;
			clickEvent.payload.click.target = listbox->id;
			clickEvent.payload.click.buttons = event->payload.mouse.buttons;
			if (! rect_test_mouse(&(listbox->bounding_rect), event->payload.mouse.x, event->payload.mouse.y))
			{
				if (listbox->focused) {
					listbox->focused = false;
					listbox->paint = true;
				}
				return;
			}
			if (listbox->event_handler != nullptr) {
				bool result = listbox->event_handler(listbox->id, &clickEvent);
				if (! result) {
					break;
				}
			}
			listbox->focused = true;
			listbox->paint = true;
			break;
		}
		case UI_EVENT_KEY: {
			if (! listbox->focused) {
				return;
			}
			uint16_t inner_height = listbox->bounding_rect.height - 2;
			if (event->payload.keyboard.keyCode == KEY_ARROW_DOWN) {
				if (listbox->cursor_y < inner_height - 1) {
					listbox->cursor_y += 1;
					listbox->paint = true;
				} else {
					if (listbox->cursor_y0 + listbox->cursor_y < listbox->num_items) {
						listbox->cursor_y0 += 1;
						listbox->paint = true;
					}
				}
			}
			if (event->payload.keyboard.keyCode == KEY_ARROW_UP) {
				if (listbox->cursor_y > 0) {
					listbox->cursor_y -= 1;
					listbox->paint = true;
				} else {
					if (listbox->cursor_y0 > 0) {
						listbox->cursor_y0 -= 1;
						listbox->paint = true;
					}
				}
			}
		}
	}
}

void range_render(ui_range_t *range)
{
	uint8_t color = range->color;
	if (range->active || range->focused) {
		color = color & 0x0f;
	}
	if (range->bounding_rect.height != 1) {
		range->bounding_rect.height = 1;
	}

	uint8_t inner_width = (range->bounding_rect.width - 2);
	float f = (float)(inner_width) / (float)(range->max - range->min + 1);

	uint8_t x0 = range->bounding_rect.x;
	uint8_t x1 = x0 + range->bounding_rect.width - 1;
	uint8_t xm = x0 + 1  + CLAMP((int)((range->value - range->min) * f + .5), 0, inner_width);
	uint8_t y  = range->bounding_rect.y;

	char stringValue[20];
	// TODO: pad string....
	sprintf(stringValue,"%d  ", inner_width, range->value, f);
	textmode_print(stringValue, x1 + 2, y, color);
	textmode_putchar_color(x0, y, CP_LEFT_TRIANGLE, color);
	textmode_hline(x0 + 1, y, xm - x0 - 1, CP_MEDIUM_SHADE, color);
	textmode_hline(xm + 1, y, inner_width - xm + x0, CP_MEDIUM_SHADE, color);
	textmode_putchar_color(xm, y, CP_SQUARE, color);
	textmode_putchar_color(x1, y, CP_RIGHT_TRIANGLE, color);
}

void range_process_events(ui_range_t *range, ui_event_t *event)
{
	// TODO
}


void component_set_focus(uint16_t count, ui_component_t *components, uint16_t id)
{
	for (uint16_t i = 0; i < count; i++) {
		bool isFocused = components[i].component.generic.id == id;
		if (isFocused != components[i].component.generic.focused) {
			components[i].component.generic.focused = isFocused;
			components[i].component.generic.paint = true;
		}
	}
}

void component_focus_next(uint16_t count, ui_component_t *components)
{
	uint16_t i, j;
	for (i = 0; i < count; i++) {
		if ((!components[i].component.generic.focused) && (i < count - 1)) {
			continue;
		}
		components[i].component.generic.focused = false;
		components[i].component.generic.paint = true;
		j = (i + 1) % count;
		components[j].component.generic.focused = true;
		components[j].component.generic.paint = true;
		return;
	}
}

void component_focus_last(uint16_t count, ui_component_t *components)
{
	uint16_t i, j;
	for (i = count - 1; i >= 0; i--) {
		if ((!components[i].component.generic.focused) && (i != 0)) {
			continue;
		}
		components[i].component.generic.focused = false;
		components[i].component.generic.paint = true;
		j = i > 0 ? (i - 1) : (count - 1);
		components[j].component.generic.focused = true;
		components[j].component.generic.paint = true;
		return;
	}
}

void component_process_events(uint16_t count, ui_component_t *components, ui_event_t *event)
{
	if (event->type == UI_EVENT_KEY) {
		if (event->payload.keyboard.keyCode == KEY_TAB) {
			component_focus_next(count, components);
			return;
		}
		if (event->payload.keyboard.keyCode == KEY_SHIFT_TAB) {
			component_focus_last(count, components);
			return;
		}
	}
	for (uint16_t i = 0; i < count; i++)
	{
		bool oldFocus = components[i].component.generic.focused;
		switch (components[i].type) {
			case COMPONENT_BUTTON: {
				button_process_events(&(components[i].component.button), event);
				break;
			}
			case COMPONENT_INPUT: {
				input_process_events(&(components[i].component.input), event);
				break;
			}
			case COMPONENT_LISTBOX: {
				listbox_process_events(&(components[i].component.listbox), event);
				break;
			}
		}

		if ((oldFocus == false) && (components[i].component.generic.focused))
		{
			// if the component processed received focus,
			// delete the focus from the other elements.
			component_set_focus(count, components, components[i].component.generic.id);
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
		case COMPONENT_BUTTON: {
			button_render(&(component->component.button));
			break;
		}

		case COMPONENT_INPUT: {
			input_render(&(component->component.input));
			break;
		}

		case COMPONENT_LISTBOX: {
			listbox_render(&(component->component.listbox));
			break;
		}

		case COMPONENT_RANGE: {
			range_render(&(component->component.range));
			break;
		}
	}
	component->component.generic.paint = false;
}

void component_render_all(uint16_t count, ui_component_t *components, bool paint_all)
{
	for (uint16_t i = 0; i < count; i++) {
		if (paint_all || components[i].component.generic.paint) {
			component_render(&(components[i]));
		}
	}
}

ui_component_t component_create_button(uint16_t id, const char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
	ui_component_t component = {0};
	ui_button_t button = {0};
	rect_t rect = {0};
	button.id = id;
	button.label = label;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	button.bounding_rect = rect;
	button.color = color;
	component.type = COMPONENT_BUTTON;
	component.component.button = button;
	return component;
}

ui_component_t component_create_input(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, const char *value, size_t maxlen)
{
	ui_component_t component = {0};
	ui_input_t input = {0};
	rect_t rect = {0};
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	input.id = id;
	input.bounding_rect = rect;
	input.color = color;
	input.value = (char *)calloc(maxlen + 1, sizeof(char));
	input.maxlen = maxlen;
	input.cursor_x0 = 0;
	input.cursor_x = 0;
	component.type = COMPONENT_INPUT;
	component.component.input = input;
	return component;
}

ui_component_t component_create_listbox(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, const char** values, uint16_t num_items)
{
	ui_component_t component = {0};
	ui_listbox_t listbox = {0};
	rect_t rect = {0};
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	listbox.id = id;
	listbox.bounding_rect = rect;
	listbox.color = color;
	listbox.values = ALLOC_TYPE(char *, num_items);
	for (uint16_t i = 0; i < num_items; i++) {
		listbox.values[i] = strdup(values[i]);
	}
	listbox.num_items = num_items;
	listbox.cursor_y0 = 0;
	listbox.cursor_y = 0;
	component.type = COMPONENT_LISTBOX;
	component.component.listbox = listbox;
	return component;
}

ui_component_t component_create_range(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, int value, int min, int max, int step)
{
	ui_component_t component = {0};
	ui_range_t range = {0};
	rect_t rect = {0};
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	range.id = id;
	range.bounding_rect = rect;
	range.color = color;
	range.value = value;
	range.min = min;
	range.max = max;
	range.step = step;
	component.type = COMPONENT_RANGE;
	component.component.range = range;
	return component;
}

void component_dispose(ui_component_t *component)
{
	switch (component->type) {
		case COMPONENT_BUTTON: {
			break;
		}
		case COMPONENT_INPUT: {
			free(component->component.input.value);
			component->component.input.value = nullptr;
			break;
		}
		case COMPONENT_LISTBOX: {
			for (uint16_t i = 0; i < component->component.listbox.num_items; i++) {
				free(component->component.listbox.values[i]);
			}
			free(component->component.listbox.values);
			component->component.listbox.values = nullptr;
		}
	}
}
