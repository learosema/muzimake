#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <stdint.h>

typedef struct rect_s {
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} rect_t;


typedef struct ui_button_s
{
	char *label;
	uint8_t color;
	rect_t bounding_rect;
	bool focused;
	bool active;
} ui_button_t;

void button_render(ui_button_t *button);



#endif
