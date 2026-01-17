#ifndef __UI_CTX_H__
#define __UI_CTX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "geometry.h"


#define MAX_EVENTS_PER_FRAME 64
#define NUM_RESERVED_EVENTS 16

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
#define UI_EVENT_MOUSEWHEEL	512

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

enum {
	UI_KEY_UP,
	UI_KEY_DOWN,
	UI_KEY_LEFT,
	UI_KEY_RIGHT,
	UI_KEY_ENTER,
	UI_KEY_ESCAPE,
	UI_KEY_TAB,
	UI_KEY_BACKSPACE,
	UI_KEY_SPACE,
	UI_KEY_SHIFT,
	UI_KEY_ALT,
	UI_KEY_CTRL,
	UI_KEY_MAX
};

#ifndef UI_TEXT_INPUT_MAX
	#define UI_TEXT_INPUT_MAX 16
#endif

typedef struct ui_mouse_state_s
{
	uint16_t x;
	uint16_t y;
	int16_t deltaX;
	int16_t deltaY;
	uint16_t buttons;
} ui_mouse_state_t;

typedef struct ui_keyboard_state_s
{
	char text[UI_TEXT_INPUT_MAX];
	int text_length;
	bool keys[UI_KEY_MAX];
} ui_keyboard_state_t;

typedef struct ui_input_s {
	ui_mouse_state_t mouse;
	ui_keyboard_state_t keyboard;
} ui_input_t;

typedef struct ui_context_s {
	ui_input_t input;
}	ui_context_t;

void ui_context_init(ui_context_t * const ctx);

void ui_context_input_begin(ui_context_t * const ctx);

void ui_context_input_end(ui_context_t * const ctx);

void ui_context_input_motion(ui_context_t * const ctx, const int x, const int y, const int deltaX, const int deltaY, const uint16_t buttons);

void ui_context_input_key(ui_context_t * const ctx, const int key_enum, const bool pressed);

void ui_context_input_mouse(ui_context_t * const ctx, const int x, const int y, const int deltaX, const int deltaY, const int buttons);

void ui_context_input_char(ui_context_t * const ctx, const char c);



/**
 * Query helpers
 */

/** Check if mouse is inside the given rectangle */
bool ui_context_is_mouse_inside_rect(ui_context_t * const ctx, const rect_t * const rect);

/** Check if mouse has moved */
bool ui_context_is_mouse_moved(ui_context_t * const ctx);

/** Check if mouse is pressed */
bool ui_context_is_mouse_pressed(ui_context_t * const ctx, const int button_id);

/** Check if mouse is released */
bool ui_context_is_mouse_released(ui_context_t * const ctx, const int button_id);

/** Check if key is pressed */
bool ui_context_is_key_pressed(ui_context_t * const ctx, const int keyCode);

/** Check if key is released */
bool ui_context_is_key_released(ui_context_t * const ctx, const int keyCode);

#ifdef __cplusplus
}
#endif
#endif
