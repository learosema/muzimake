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

bool rect_test_point_in_bounds(rect_t *rect, uint8_t x, uint8_t y);
ui_button_t button_create(char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
bool button_test_point_in_bounds(ui_button_t * button, uint8_t x, uint8_t y);
bool button_test_mouse(ui_button_t * button, uint16_t mouseX, uint16_t mouseY);
void button_render(ui_button_t *button);
void button_process_events(uint8_t buttonCount, ui_button_t *buttons, ui_event_t *event);

void input_render(ui_input_t *input);

void component_render(ui_component_t *component);

#endif
