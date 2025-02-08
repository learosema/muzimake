#ifndef __UI_EVENT_H__
#define __UI_EVENT_H__

#include <stdint.h>

#define UI_EVENT_KEYDOWN    1
#define UI_EVENT_KEYUP			2
#define UI_EVENT_MOUSEDOWN  4
#define UI_EVENT_MOUSEUP    8
#define UI_EVENT_CLICK			16

typedef struct ui_mouse_event_s
{
	uint16_t mouseX;
	uint16_t mouseY;
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
	};
} ui_event_t;

void poll_event(ui_event_t *event);



#endif
