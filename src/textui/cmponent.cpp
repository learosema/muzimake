#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "cmponent.h"
#include "textmode.h"
#include "macros.h"
#include "vendor/cp437.h"
#include "helper/log.h"

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

rect_t get_clientrect(rect_t *bounding_rect)
{
	rect_t clientrect = *bounding_rect;
	if (clientrect.height > 2 && clientrect.width > 2)
	{
		clientrect.x += 1;
		clientrect.y += 1;
		clientrect.width -= 2;
		clientrect.height -= 2;
	}
	return clientrect;
}

void ui_handle_mouseup(ui_generic_t *generic, ui_event_t *event) {
	if (event->type != UI_EVENT_MOUSEUP) {
		return;
	}
	ui_event_t clickEvent = {0};
	clickEvent.type = UI_EVENT_CLICK;
	clickEvent.payload.click.target = generic->id;
	clickEvent.payload.click.buttons = event->payload.mouse.buttons;
	if (! rect_test_mouse(&(generic->bounding_rect), event->payload.mouse.x, event->payload.mouse.y))
	{
		if (generic->focused) {
			generic->focused = false;
			generic->paint = true;
		}
		return;
	}
	if (generic->event_handler != nullptr) {
		bool result = generic->event_handler(generic->id, &clickEvent);
		if (! result) {
			return;
		}
	}
	generic->focused = true;
	generic->paint = true;
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

void label_render(ui_label_t *label)
{
	textmode_print(label->label, label->bounding_rect.x, label->bounding_rect.y, label->color);
}

void label_process_events(ui_label_t *label, ui_event_t * event) {
	if (event->type == UI_EVENT_MOUSEUP) {
		ui_handle_mouseup((ui_generic_t *)label, event);
 	}
}

void input_render(ui_input_t *input)
{
	APP_LOG("input render %d", input->focused);
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
	switch (event->type) {
		case UI_EVENT_MOUSEUP: {
			ui_handle_mouseup((ui_generic_t *)input, event);
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
	APP_LOG("listbox_render, offset %d, cursor: %d, items: %d", listbox->cursor_y, listbox->cursor_y0, listbox->num_items);
	uint8_t lbcolor = listbox->color;
	const uint8_t symbol_topleft = listbox->focused ?
		CP_THICK_RIGHT_THICK_DOWN : CP_THIN_RIGHT_THIN_DOWN;
	const uint8_t symbol_horizontal = listbox->focused ?
		CP_THICK_HORIZONTAL : CP_THIN_HORIZONTAL;
	const uint8_t symbol_topright = listbox->focused ?
		CP_THICK_LEFT_THICK_DOWN : CP_THIN_LEFT_THIN_DOWN;
	const uint8_t symbol_vertical = listbox->focused ?
		CP_THICK_VERTICAL : CP_THIN_VERTICAL;
	const uint8_t symbol_bottomright = listbox->focused ?
		CP_THICK_LEFT_THICK_UP : CP_THIN_LEFT_THIN_UP;
	const uint8_t symbol_bottomleft= listbox->focused ?
		CP_THICK_RIGHT_THICK_UP : CP_THIN_RIGHT_THIN_UP;

	rect_t clientrect = get_clientrect(&(listbox->bounding_rect));

	uint8_t x0 = listbox->bounding_rect.x;
	uint8_t y0 = listbox->bounding_rect.y;
	uint8_t x1 = x0 + listbox->bounding_rect.width - 1;
	uint8_t y1 = y0 + listbox->bounding_rect.height - 1;
	uint8_t w = listbox->bounding_rect.width;
	uint8_t h = listbox->bounding_rect.height;

	textmode_putchar_color(x0, y0, symbol_topleft, lbcolor);
	textmode_putchar_color(x1, y0, symbol_topright, lbcolor);
	textmode_putchar_color(x0, y1, symbol_bottomleft, lbcolor);
	textmode_putchar_color(x1, y1, symbol_bottomright, lbcolor);

	if (w > 2) {
		textmode_hline(x0 + 1, y0, w - 2, symbol_horizontal, lbcolor);
		textmode_hline(x0 + 1, y1, w - 2, symbol_horizontal, lbcolor);
	}
	if (h > 2) {
		textmode_vline(x0, y0 + 1, h - 2, symbol_vertical, lbcolor);
		if (listbox->num_items == 0 || clientrect.height >= listbox->num_items) {
			// do not show a scroll line when the list is empty or the listbox is
			// big enough
			textmode_vline(x1, y0 + 1, h - 2, symbol_vertical, lbcolor);
		} else {
			textmode_putchar_color(x1, y0 + 1, CP_UP_TRIANGLE, lbcolor);
			textmode_putchar_color(x1, y0 + h - 2, CP_DOWN_TRIANGLE, lbcolor);
			uint8_t scrollbarheight = clientrect.height - 2; // the bar height without arrows
			float thumb_f = (float)(clientrect.height) / (float)(listbox->num_items);
			uint8_t thumb_h = CLAMP(
				(0.5f + (float)(scrollbarheight) * thumb_f), 1, scrollbarheight);
			uint8_t thumb_y0 = 2 + CLAMP((uint8_t)(
				(float)(listbox->cursor_y0) / (float)(listbox->num_items) * (float)(scrollbarheight) + 0.5
			), 0, (h-4));

			for (uint8_t scroll_y = 2; scroll_y < h - 2; scroll_y++) {
				textmode_putchar_color(x1, y0 + scroll_y,
					((scroll_y >= thumb_y0) &&
					(scroll_y <(thumb_y0 + thumb_h))) ? CP_BLOCK : CP_MEDIUM_SHADE, lbcolor);
			}
		}
	}

	uint8_t inner_width = listbox->bounding_rect.width - 2;
	uint8_t inner_height = listbox->bounding_rect.height - 2;
	uint8_t select_color = 0x70;
	for (uint8_t y = 0; y < inner_height; y++)
	{
		uint16_t yy = listbox->cursor_y0 + y;
		uint8_t color = yy == (listbox->cursor_y0 + listbox->cursor_y) ?
			select_color : lbcolor;
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
			ui_handle_mouseup((ui_generic_t *)listbox, event);
			rect_t clientrect = get_clientrect(&(listbox->bounding_rect));
			if (rect_test_mouse(&clientrect, event->payload.mouse.x, event->payload.mouse.y)) {
				uint8_t y = (uint8_t)(event->payload.mouse.y / 8) - clientrect.y;
				if (y + listbox->cursor_y0 < listbox->num_items) {
					listbox->cursor_y = y;
					listbox->paint = true;
				}
			}
			break;
		}
		case UI_EVENT_KEY: {
			if (! listbox->focused) {
				return;
			}
			uint16_t inner_height = listbox->bounding_rect.height - 2;
			if (event->payload.keyboard.keyCode == KEY_ARROW_DOWN) {
				if (listbox->cursor_y0 + listbox->cursor_y < listbox->num_items - 1) {
					if (listbox->cursor_y < inner_height - 1) {
						listbox->cursor_y += 1;
					} else {
						listbox->cursor_y0 += 1;
					}
					listbox->paint = true;
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
	if (range->bounding_rect.height != 1 && range->bounding_rect.height != 3) {
		range->bounding_rect.height = 3;
	}
	if (range->bounding_rect.width < 10) {
		range->bounding_rect.width = 10;
	}

	rect_t clientrect = get_clientrect(&(range->bounding_rect));
	clientrect.width -= 4;
  if (range->bounding_rect.height == 3)
	{

		if (range->focused) {
			textmode_dblrect(
				range->bounding_rect.x,
				range->bounding_rect.y,
				range->bounding_rect.width,
				range->bounding_rect.height,
				color
			);
		} else {
			textmode_rect(
				range->bounding_rect.x,
				range->bounding_rect.y,
				range->bounding_rect.width,
				range->bounding_rect.height,
				color
			);
		}
	}

	uint8_t slider_width = (clientrect.width - 2);
	float f = (float)(slider_width) / (float)(range->max - range->min + 1);

	uint8_t x0 = clientrect.x;
	uint8_t x1 = x0 + clientrect.width - 1;
	uint8_t xm = x0 + 1  + CLAMP((int)((float)(range->value - range->min) * f + .5f), 0, slider_width);
	uint8_t y  = clientrect.y;

	char stringValue[20];
	sprintf(stringValue,"%4d", range->value);

	textmode_print(stringValue, x1 + 1, y, color);
	textmode_putchar_color(x0, y, CP_LEFT_TRIANGLE, color);
	textmode_hline(x0 + 1, y, xm - x0 - 1, CP_MEDIUM_SHADE, color);
	textmode_hline(xm + 1, y, slider_width - xm + x0, CP_MEDIUM_SHADE, color);
	textmode_putchar_color(xm, y, CP_SQUARE, color);
	textmode_putchar_color(x1, y, CP_RIGHT_TRIANGLE, color);
}

void range_process_events(ui_range_t *range, ui_event_t *event)
{
	switch (event->type) {
		case UI_EVENT_MOUSEMOVE:
		case UI_EVENT_MOUSEDOWN: {
			if (event->payload.mouse.buttons & 1 > 0) {
				rect_t clientrect = get_clientrect(&(range->bounding_rect));
				clientrect.width -= 4;
				if ((clientrect.width<3) || (clientrect.height < 1)) {
					return;
				}
				if (rect_test_mouse(&clientrect, event->payload.mouse.x, event->payload.mouse.y)) {
					uint8_t x = (uint8_t)(event->payload.mouse.x / 8);
					x = x - clientrect.x - 1;
					if (x >= 0 && x < clientrect.width - 1) {
						float inner_width = (float)(clientrect.width - 2);
						float f = (float)(x) / inner_width;
						int newValue = range->min + (int)(f * (float)(range->max - range->min + 1) + 0.5f);
						range->value = CLAMP(newValue, range->min, range->max);
						range->paint = true;
					}
				}
			}
			break;
		}
		case UI_EVENT_MOUSEUP: {
			ui_handle_mouseup((ui_generic_t *)range, event);
			break;
		}
	}
	if (event->type == UI_EVENT_KEY) {
		if (range->focused == false) {
			return;
		}
		switch (event->payload.keyboard.keyCode) {
			case KEY_ARROW_LEFT: {
				range->value = MAX(range->min, range->value - range->step);
				range->paint = true;
				break;
			}
			case KEY_ARROW_RIGHT: {
				range->value = MIN(range->max, range->value + range->step);
				range->paint = true;
				break;
			}
		}
	}
}

ui_component_t *component_get_by_id(uint16_t count, ui_component_t *components, uint16_t id)
{
	for (uint16_t i = 0; i < count; i++) {
		if (components[i].component.generic.id == id) {
			return &(components[i]);
		}
	}
	return 0;
}

int component_find_focused(uint16_t count, ui_component_t *components)
{
	for (uint16_t i = 0; i < count; i++) {
		if (components[i].component.generic.focused) {
			return i;
		}
	}
	return -1;
}

int component_index_by_id(uint16_t count, ui_component_t *components, uint16_t id)
{
	uint16_t i = 0;
	for (i = 0; i < count; i++) {
		if (components[i].component.generic.id == id) {
			return (int)i;
		}
	}
	return -1;
}

void component_set_focus(uint16_t count, ui_component_t *components, uint16_t id)
{
	for (uint16_t i = 0; i < count; i++) {
		ui_component_t * component = &(components[i]);
		bool isFocused = component->component.generic.id == id;
		if (isFocused != component->component.generic.focused) {
			component->component.generic.focused = isFocused;
			component->component.generic.paint = true;
		}
	}
}

void component_focus_next(uint16_t count, ui_component_t *components)
{
	if (count == 0) {
		// no elements at all
		return;
	}
	int curr = component_find_focused(count, components);
	int next = curr >= 0 ? (curr + 1) % count : 0;
	bool found = false;

	for (int it = 0; it < count; it++) {
		if (components[next].type != COMPONENT_LABEL) {
			found = true;
			break;
		}
		next = (next + 1) % count;
	}

	if (!found) {
		// There is nothing to focus
		return;
	}

	if (curr == next) {
		// The current element is the only one to focus.
		return;
	}

	if (curr >= 0) {
		components[curr].component.generic.focused = false;
		components[curr].component.generic.paint = true;
	}

	components[next].component.generic.focused = true;
	components[next].component.generic.paint = true;
}

void component_focus_prev(uint16_t count, ui_component_t *components)
{
	if (count == 0) {
		// no elements at all
		return;
	}
	int curr = component_find_focused(count, components);
	int prev = curr > 0 ? (curr - 1) : (count - 1);

	bool found = false;

	for (int it = 0; it < count; it++) {
		if (components[prev].type != COMPONENT_LABEL) {
			found = true;
			break;
		}
		prev = prev > 0 ? (prev - 1) : (count - 1);
	}

	if (!found) {
		// There is nothing to focus
		return;
	}

	if (curr == prev) {
		// The current element is the only one to focus.
		return;
	}

	if (curr >= 0) {
		components[curr].component.generic.focused = false;
		components[curr].component.generic.paint = true;
	}

	components[prev].component.generic.focused = true;
	components[prev].component.generic.paint = true;
}

void component_process_events(uint16_t count, ui_component_t *components, ui_event_t *event)
{
	if (event->type == UI_EVENT_KEY) {
		if (event->payload.keyboard.keyCode == KEY_TAB) {
			component_focus_next(count, components);
			return;
		}
		if (event->payload.keyboard.keyCode == KEY_SHIFT_TAB) {
			component_focus_prev(count, components);
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
			case COMPONENT_LABEL: {
				label_process_events(&(components[i].component.label), event);
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
			case COMPONENT_RANGE: {
				range_process_events(&(components[i].component.range), event);
				break;
			}
		}

		if ((oldFocus == false) && (components[i].component.generic.focused))
		{
			uint8_t focusId = components[i].component.generic.id;
			// if the component processed received focus,
			// delete the focus from the other elements.
			component_set_focus(count, components, focusId);
		}
	}

	// if a label has been focused, then change the focus to the
	// assoc element
	int focused_idx = component_find_focused(count, components);
	if (focused_idx > -1 && focused_idx < count && components[focused_idx].type == COMPONENT_LABEL) {
		uint16_t for_id = components[focused_idx].component.label.forId;
		ui_component_t *associated_component = component_get_by_id(count, components, for_id);
		if (associated_component) {
			components[focused_idx].component.generic.focused = false;
			components[focused_idx].component.generic.paint = false;
			associated_component->component.generic.focused = true;
			associated_component->component.generic.paint = true;
		}
	}
}

void component_render(ui_component_t *component)
{
	if (component == nullptr || component->component.generic.paint == false) {
		return;
	}
	switch (component->type)
	{
		case COMPONENT_BUTTON: {
			button_render(&(component->component.button));
			break;
		}
		case COMPONENT_LABEL: {
			label_render(&(component->component.label));
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
		if (paint_all) {
			components[i].component.generic.paint = true;
		}
		component_render(&(components[i]));
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
	component.component.generic.paint = true;
	return component;
}

ui_component_t component_create_label(uint16_t id, uint16_t forId, const char *label, uint8_t x, uint8_t y, uint8_t color)
{
	ui_component_t component = {0};
	ui_label_t label_el = {0};
	rect_t rect = {0};
	label_el.id = id;
	label_el.forId = forId;
	label_el.label = label;
	rect.x = x;
	rect.y = y;
	rect.width = strlen(label);
	rect.height = 1;
	label_el.bounding_rect = rect;
	label_el.color = color;
	component.type = COMPONENT_LABEL;
	component.component.label = label_el;
	component.component.generic.paint = true;
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
	component.component.generic.paint = true;
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
	component.component.generic.paint = true;
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
	component.component.generic.paint = true;
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
