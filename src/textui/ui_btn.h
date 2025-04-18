#ifndef __UI_BTN_H__
#define __UI_BTN_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ui_common.h"

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

void button_render(ui_button_t *button);
void button_process_events(ui_button_t *button, ui_event_t *event);

#ifdef __cplusplus
}
#endif
#endif
