#ifndef __UI_LABEL_H__
#define __UI_LABEL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ui_common.h"

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

void label_render(ui_label_t *label);
void label_process_events(ui_label_t *label, ui_event_t * event);

#ifdef __cplusplus
}
#endif
#endif
