#ifndef __UI_HELPER_H__
#define __UI_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#include "ui_event.h"

#define COMPONENT_BUTTON  1
#define COMPONENT_LABEL   2
#define COMPONENT_INPUT   3
#define COMPONENT_LISTBOX 4
#define COMPONENT_RANGE   5
#define COMPONENT_PIANO   6
#define COMPONENT_SHEET   7

#define INPUT_CURSOR_COLOR 0x3f

typedef bool (*ui_event_handler_t)(uint16_t, ui_event_t *);

typedef struct rect_s
{
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} rect_t;

typedef struct ui_generic_s
{
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	ui_event_handler_t event_handler;
} ui_generic_t;

bool rect_test_point_in_bounds(rect_t * rect, uint8_t x, uint8_t y);
bool rect_test_mouse(rect_t * rect, uint16_t mouseX, uint16_t mouseY);
rect_t get_clientrect(const rect_t *bounding_rect);
void ui_handle_mouseup(ui_generic_t *generic, ui_event_t *event);

#ifdef __cplusplus
}
#endif
#endif
