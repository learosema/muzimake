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

#define KEY_ARROW_LEFT      0x4b00
#define KEY_ARROW_RIGHT     0x4d00
#define KEY_ARROW_UP        0x4800
#define KEY_ARROW_DOWN      0x5000
#define KEY_BACKSPACE       0x08
#define KEY_ENTER           0x0D
#define KEY_TAB             0x09
#define KEY_ESCAPE          0x1D

typedef struct ui_mouse_event_s
{
	uint16_t x;
	uint16_t y;
	uint16_t buttons;
} ui_mouse_event_t;

typedef struct ui_keyboard_event_s
{
	uint16_t keyCode;
} ui_keyboard_event_t;

typedef struct ui_click_event_s {
	uint16_t target;
	uint8_t buttons;
} ui_click_event_t;

typedef struct ui_event_s
{
	uint8_t type;
	union event_payload {
		ui_mouse_event_t mouse;
		ui_keyboard_event_t keyboard;
		ui_click_event_t click;
	} payload;
} ui_event_t;

void poll_event(ui_event_t *event);



#endif
