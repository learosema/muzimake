#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <stdint.h>
#include "ui_event.h"

#define COMPONENT_BUTTON 1
#define COMPONENT_INPUT  2

#define INPUT_CURSOR_COLOR 0x3f

typedef struct rect_s {
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} rect_t;

typedef struct ui_button_s
{
	uint8_t id;
	rect_t bounding_rect;
	char *label;
	uint8_t color;
	bool active;
	bool focused;
} ui_button_t;

typedef struct ui_input_s {
	uint8_t id;
	rect_t bounding_rect;
	char *label;
	uint8_t color;
	bool active;
	bool focused;
	char* value;
	int maxlen;
	int cursor_x0;
	int cursor_x;
} ui_input_t;

typedef struct ui_component_s
{
	uint8_t type;
	union component_data {
		ui_button_t button;
		ui_input_t input;
	} component;
} ui_component_t;

void component_process_events(uint8_t componentCount, ui_component_t *components, ui_event_t *event);
void component_render(ui_component_t *component);
ui_component_t component_create_button(uint8_t id, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
ui_component_t component_create_input(uint8_t id, char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, char *value, size_t maxlen);
void component_dispose(ui_component_t *component);

#endif
