#ifndef __UI_LSBOX_H__
#define __UI_LSBOX_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ui_common.h"

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

void listbox_render(ui_listbox_t *listbox);
void listbox_process_events(ui_listbox_t *listbox, ui_event_t *event);

#ifdef __cplusplus
}
#endif
#endif
