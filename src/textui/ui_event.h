#ifndef __UI_EVENT_H__
#define __UI_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "geometry.h"


#define MAX_EVENTS_PER_FRAME 16

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
#define KEY_ESCAPE          0x1B
#define KEY_SPACE						0x20
#define KEY_ALT_X           0x2D00
#define KEY_ALT_I           0x1700

// these are the codes you get for the KEYDOWN/KEYUP events
// they are different from the codes for the "KEY" event
#define KEY_SCANCODE_ENTER			0x1c
#define KEY_SCANCODE_SPACE			0x39

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

typedef struct ui_event_pool_s
{
	ui_event_t * events;
	size_t size;
	size_t capacity;
} ui_event_pool_t;

void ui_event_pool_init(ui_event_pool_t * const pool, const size_t capacity);
void ui_event_pool_dispose(ui_event_pool_t * const pool);
void ui_event_pool_add_event(ui_event_pool_t * const pool, const ui_event_t * const event);

void ui_event_begin(ui_event_pool_t * const pool);
void ui_event_click(ui_event_pool_t * const pool);
void ui_event_mouse_wheel(ui_event_pool_t * const pool, const int delta);
void ui_event_mouse_press(ui_event_pool_t * const pool, const int button_id);
void ui_event_mouse_release(ui_event_pool_t * const pool, const int button_id);
void ui_event_mouse_move(ui_event_pool_t * const pool, const int x, const int y, const int deltaX, const int deltaY);
void ui_event_key(ui_event_pool_t * const pool, const int keyCode, const bool pressed);
void ui_event_end(ui_event_pool_t * const pool);

bool ui_event_is_mouse_inside_rect(ui_event_pool_t * const pool, const rect_t * const rect);
bool ui_event_is_mouse_moved(ui_event_pool_t * const pool);
bool ui_event_is_mouse_pressed(ui_event_pool_t * const pool, const int button_id);
bool ui_event_is_mouse_released(ui_event_pool_t * const pool, const int button_id);
bool ui_event_is_key_pressed(ui_event_pool_t * const pool, const int keyCode);
bool ui_event_is_key_released(ui_event_pool_t * const pool, const int keyCode);


#ifdef __cplusplus
}
#endif
#endif
