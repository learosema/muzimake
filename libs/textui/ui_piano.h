#ifndef __UI_PIANO_H__
#define __UI_PIANO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ui_common.h"

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

void piano_render(ui_piano_t *piano);
void piano_process_events(ui_piano_t *piano, ui_event_t *event);

#ifdef __cplusplus
}
#endif
#endif
