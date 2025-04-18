#ifndef __UI_INPUT_H__
#define __UI_INPUT_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ui_common.h"


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


void input_render(ui_input_t *input);
void input_process_events(ui_input_t *input, ui_event_t *event);


#ifdef __cplusplus
}
#endif
#endif
