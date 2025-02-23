#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <stdint.h>
#include "ui_event.h"

#define COMPONENT_BUTTON 1
#define COMPONENT_INPUT  2

#define INPUT_CURSOR_COLOR 0x3f

typedef bool (*ui_event_handler_t)(uint16_t, ui_event_t *);

typedef struct rect_s
{
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} rect_t;

typedef struct ui_generic_s
{
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
} ui_generic_t;


typedef struct ui_button_s
{
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	const char *label;
} ui_button_t;

typedef struct ui_input_s {
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	char* value;
	int maxlen;
	int cursor_x0;
	int cursor_x;
	bool overwrite;
} ui_input_t;

typedef struct ui_component_s
{
	uint16_t type;
	union component_data {
		ui_generic_t generic;
		ui_button_t button;
		ui_input_t input;
	} component;
} ui_component_t;

typedef struct ui_state_s
{
	uint16_t count;
	ui_component_t *components;
	ui_component_t *focusedElement;
} ui_state_t;

void component_process_events(uint16_t count, ui_component_t *components, ui_event_t *event);
void component_render(ui_component_t *component);
void component_render_all(uint16_t count, ui_component_t *components, bool paint_all);
ui_component_t component_create_button(uint16_t id, const char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
ui_component_t component_create_input(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, char *value, size_t maxlen);
void component_dispose(ui_component_t *component);
void component_set_focus(uint16_t componentCount, ui_component_t *components, uint16_t id);
void component_focus_next(uint16_t count, ui_component_t *components);
void component_focus_last(uint16_t count, ui_component_t *components);
#endif
