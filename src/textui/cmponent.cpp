#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "cmponent.h"
#include "textmode.h"
#include "macros.h"
#include "vendor/cp437.h"
#include "helper/log.h"

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
	APP_LOG("component_focus_next, count: %d", count);
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
	APP_LOG("component_focus_prev, count: %d", count);
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
			APP_LOG("TAB pressed");
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
			case COMPONENT_PIANO: {
				piano_process_events(&(components[i].component.piano), event);
				break;
			}
			case COMPONENT_SHEET: {
				sheet_process_events(&(components[i].component.sheet), event);
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

		case COMPONENT_PIANO: {
			piano_render(&(component->component.piano));
			break;
		}

		case COMPONENT_SHEET: {
			sheet_render(&(component->component.sheet));
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

ui_component_t component_create_sheet(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, uint8_t rows, uint8_t cols)
{
	ui_component_t component = {0};
	ui_sheet_t sheet = {0};
	rect_t rect = {0};

	pattern_t pattern = {0};
	pattern.num_cols = cols;
	pattern.num_rows = rows;
	pattern.data = ALLOC_TYPE(pattern_entry_t, rows*cols);
	memset(pattern.data, 0, cols * rows * sizeof(pattern_entry_t));

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	sheet.id = id;
	sheet.bounding_rect = rect;
	sheet.color = color;
	// TODO add data
	component.type = COMPONENT_SHEET;
	component.component.sheet = sheet;
	component.component.generic.paint = true;
	component.component.sheet.pattern = pattern;
	return component;
}

ui_component_t component_create_piano(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color)
{
	ui_component_t component = {0};
	ui_piano_t piano = {0};
	rect_t rect = {0};
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	piano.id = id;
	piano.bounding_rect = rect;
	piano.color = color;

	component.type = COMPONENT_PIANO;
	component.component.piano = piano;
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
		case COMPONENT_SHEET: {
			free(component->component.sheet.pattern.data);
			component->component.sheet.pattern.data = nullptr;
		}
	}
}
