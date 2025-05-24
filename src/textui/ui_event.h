#ifndef __UI_EVENT_H__
#define __UI_EVENT_H__

#include <stdint.h>
#include "mouse.h"


#define EVENT_MOUSEMOVE    1
#define EVENT_MOUSEDOWN_L  2
#define EVENT_MOUSEUP_L    4
#define EVENT_MOUSEDOWN_R  8
#define EVENT_MOUSEUP_R    16

#define UI_EVENT_NONE       0

#define UI_EVENT_MOUSE 31
#define UI_EVENT_MOUSEMOVE  1
#define UI_EVENT_MOUSEDOWN  2
#define UI_EVENT_MOUSEUP    4
#define UI_EVENT_MOUSEDOWN_R 8
#define UI_EVENT_MOUSEUP_R  16

#define UI_EVENT_KEYDOWN    32
#define UI_EVENT_KEYUP      64
#define UI_EVENT_KEY        128
#define UI_EVENT_CLICK			256

#define KEY_ARROW_LEFT      0x4B00
#define KEY_ARROW_RIGHT     0x4D00
#define KEY_ARROW_UP        0x4800
#define KEY_ARROW_DOWN      0x5000
#define KEY_ALT_ARROW_LEFT  0x9B00
#define KEY_ALT_ARROW_RIGHT 0x9D00
#define KEY_ALT_ARROW_UP    0x9800
#define KEY_ALT_ARROW_DOWN  0xA000
#define KEY_BACKSPACE       0x08
#define KEY_ENTER           0x0D
#define KEY_TAB             0x09
#define KEY_SHIFT_TAB				0x0F00
#define KEY_ESCAPE          0x1D
#define KEY_ALT_X           0x2D00
#define KEY_ALT_I           0x1700

typedef struct ui_mouse_event_s
{
	uint16_t x;
	uint16_t y;
	int16_t deltaX;
	int16_t deltaY;
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
	uint16_t type;
	union event_payload {
		ui_mouse_event_t mouse;
		ui_keyboard_event_t keyboard;
		ui_click_event_t click;
	} payload;
} ui_event_t;

uint8_t event_poll(ui_event_t *events, uint16_t offset, uint16_t max_events);
void event_clear(ui_event_t *event);
void event_init();
void event_shutdown();

#endif
