#ifndef __UI_BTN_H__
#define __UI_BTN_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "geometry.h"
#include <stdint.h>
#include <stdbool.h>
#include <list.h>
#include "txtbuffer.h"
#include "ui_base.h"
#include "ui_ctx.h"

typedef struct ui_button_s
{
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	/** whether button is pressed */
	bool active;
	/** whether button is focused */
	bool focused;
	/** whether button needs repaint */
	bool paint;
	event_handler_t event_handler;
	const char *label;
} ui_button_t;

/** renders the button */
void button_render(textbuffer_t * const buffer, const ui_button_t * const button);

// void button_process_events(ui_button_t *button, ui_event_t *event);

/** sets the focus state of the button */
void button_focus(ui_button_t * const button, const bool focus);

/** sets the active state of the button */
void button_active(ui_button_t * const button, const bool active);

void button_behaviour(const ui_context_t * const ctx, ui_button_t * const button);

#ifdef __cplusplus
}
#endif
#endif
