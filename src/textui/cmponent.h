#ifndef __BUTTON_H__
#define __BUTTON_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ui_event.h"

#define COMPONENT_BUTTON  1
#define COMPONENT_LABEL   2
#define COMPONENT_INPUT   3
#define COMPONENT_LISTBOX 4
#define COMPONENT_RANGE   5
#define COMPONENT_PIANO   6
#define COMPONENT_SHEET   7

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

typedef struct ui_label_s
{
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	const char *label;
	uint8_t forId;
} ui_label_t;

typedef struct ui_input_s
{
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

typedef struct ui_listbox_s {
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	char** values;
	int num_items;
	int cursor_y0;
	int cursor_y;
} ui_listbox_t;

typedef struct ui_range_s {
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	int value;
	int min;
	int max;
	int step;
} ui_range_t;

typedef struct ui_piano_s {
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	uint8_t num_keys;
	uint8_t octave;
	uint16_t pressed_keys;
} ui_piano_t;

typedef void * pattern_t; // TODO

typedef struct ui_sheet_s {
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	pattern_t *pattern;
	int offset_x;
	int offset_y;
	int cursor_x;
	int cursor_y;
	int cursor_input_x;
} ui_sheet_t;

typedef struct ui_component_s
{
	uint16_t type;
	union component_data {
		ui_generic_t generic;
		ui_button_t button;
		ui_label_t label;
		ui_input_t input;
		ui_listbox_t listbox;
		ui_range_t range;
		ui_piano_t piano;
		ui_sheet_t sheet;
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
ui_component_t component_create_label(uint16_t id, uint16_t forId, const char *label, uint8_t x, uint8_t y, uint8_t color);
ui_component_t component_create_input(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, const char *value, size_t maxlen);
ui_component_t component_create_listbox(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, const char** values, uint16_t num_items);
ui_component_t component_create_range(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, int value, int min, int max, int step);
ui_component_t component_create_piano(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);

void component_dispose(ui_component_t *component);

void component_set_focus(uint16_t componentCount, ui_component_t *components, uint16_t id);
void component_focus_next(uint16_t count, ui_component_t *components);
void component_focus_last(uint16_t count, ui_component_t *components);

#ifdef __cplusplus
}
#endif
#endif
