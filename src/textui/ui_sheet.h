#ifndef __UI_SHEET_H__
#define __UI_SHEET_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ui_common.h"

typedef struct pattern_entry_s
{
	uint8_t note;
	uint8_t velocity;
	uint8_t effects;
	uint8_t reserved;
} pattern_entry_t;

typedef struct pattern_s
{
	uint8_t num_rows;
	uint8_t num_cols;
	pattern_entry_t *data;
} pattern_t;

typedef struct ui_sheet_s {
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	pattern_t pattern;
	uint8_t offset_x;
	uint8_t offset_y;
	uint8_t cursor_x;
	uint8_t cursor_y;
	uint8_t cursor_input_x;
} ui_sheet_t;

void sheet_render(ui_sheet_t *sheet);
void sheet_process_events(ui_sheet_t *sheet, ui_event_t *event);;

#ifdef __cplusplus
}
#endif
#endif
