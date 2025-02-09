#ifndef __UI_EVENT_H__
#define __UI_EVENT_H__

#include <stdint.h>
#include "mouse.h"

#define UI_EVENT_KEYDOWN    1
#define UI_EVENT_KEYUP			2
#define UI_EVENT_KEY				4
#define UI_EVENT_MOUSEDOWN  8
#define UI_EVENT_MOUSEUP    16
#define UI_EVENT_CLICK			32
#define UI_EVENT_MOUSEMOVE	64

typedef struct ui_mouse_event_s
{
	uint16_t x;
	uint16_t y;
	uint16_t buttons;
} ui_mouse_event_t;

typedef struct ui_keyboard_event_s
{
	uint8_t keyCode;
} ui_keyboard_event_t;

typedef struct ui_event_s
{
	uint8_t type;
	union event_payload {
		ui_mouse_event_t mouse;
		ui_keyboard_event_t keyboard;
	} payload;
} ui_event_t;

void poll_event(ui_event_t *event);



#endif
