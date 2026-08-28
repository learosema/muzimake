#ifndef __UI_RANGE_H__
#define __UI_RANGE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ui_common.h"

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

void range_render(ui_range_t *range);
void range_process_events(ui_range_t *range, ui_event_t *event);

#ifdef __cplusplus
}
#endif
#endif
