#ifndef __CMPONENT_H__
#define __CMPONENT_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ui_event.h"
#include "ui_common.h"
#include "ui_btn.h"
#include "ui_label.h"
#include "ui_input.h"
#include "ui_lsbox.h"
#include "ui_range.h"
#include "ui_piano.h"
#include "ui_sheet.h"

#define COMPONENT_BUTTON  1
#define COMPONENT_LABEL   2
#define COMPONENT_INPUT   3
#define COMPONENT_LISTBOX 4
#define COMPONENT_RANGE   5
#define COMPONENT_PIANO   6
#define COMPONENT_SHEET   7

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

ui_component_t *component_get_by_id(uint16_t count, ui_component_t *components, uint16_t id);
void component_set_focus(uint16_t componentCount, ui_component_t *components, uint16_t id);
void component_focus_next(uint16_t count, ui_component_t *components);
void component_focus_last(uint16_t count, ui_component_t *components);
int component_find_focused(uint16_t count, ui_component_t *components);
int component_index_by_id(uint16_t count, ui_component_t *components, uint16_t id);

ui_component_t component_create_button(uint16_t id, const char *label, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
ui_component_t component_create_label(uint16_t id, uint16_t forId, const char *label, uint8_t x, uint8_t y, uint8_t color);
ui_component_t component_create_input(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, const char *value, size_t maxlen);
ui_component_t component_create_listbox(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, const char** values, uint16_t num_items);
ui_component_t component_create_range(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, int value, int min, int max, int step);
ui_component_t component_create_piano(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
ui_component_t component_create_sheet(uint16_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color, uint8_t rows, uint8_t cols);

void component_dispose(ui_component_t *component);


#ifdef __cplusplus
}
#endif
#endif
