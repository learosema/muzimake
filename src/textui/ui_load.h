#ifndef __UI_SHEET_H__
#define __UI_SHEET_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ui_common.h"
#include "helper/list.h"

typedef struct ui_load_s {
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
	linked_list_t *current_dir;
	uint8_t offset_x;
	int8_t offset_y;
	uint8_t cursor_x;
	uint8_t cursor_y;
} ui_load_t;

void ui_load_render(ui_load_t *load_state);
void ui_load_process_events(ui_load_t *load_state, ui_event_t *event);

#ifdef __cplusplus
}
#endif
#endif
