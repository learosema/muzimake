#include <stdio.h>
#include <string.h>

#include "cp437.h"
#include "textmode.h"
#include "macros.h"
#include "ui_range.h"

void range_render(ui_range_t *range)
{
	uint8_t color = range->color;
	if (range->bounding_rect.height != 1 && range->bounding_rect.height != 3) {
		range->bounding_rect.height = 3;
	}
	if (range->bounding_rect.width < 10) {
		range->bounding_rect.width = 10;
	}

	rect_t clientrect = get_clientrect(&(range->bounding_rect));
	clientrect.width -= 4;
  if (range->bounding_rect.height == 3)
	{

		if (range->focused) {
			textmode_dblrect(
				range->bounding_rect.x,
				range->bounding_rect.y,
				range->bounding_rect.width,
				range->bounding_rect.height,
				color
			);
		} else {
			textmode_rect(
				range->bounding_rect.x,
				range->bounding_rect.y,
				range->bounding_rect.width,
				range->bounding_rect.height,
				color
			);
		}
	}

	uint8_t slider_width = (clientrect.width - 2);
	float f = (float)(slider_width) / (float)(range->max - range->min + 1);

	uint8_t x0 = clientrect.x;
	uint8_t x1 = x0 + clientrect.width - 1;
	uint8_t xm = x0 + 1  + CLAMP((int)((float)(range->value - range->min) * f + .5f), 0, slider_width);
	uint8_t y  = clientrect.y;

	char stringValue[20];
	sprintf(stringValue,"%4d", range->value);

	textmode_print(stringValue, x1 + 1, y, color);
	textmode_putchar_color(x0, y, CP_LEFT_TRIANGLE, color);
	textmode_hline(x0 + 1, y, xm - x0 - 1, CP_MEDIUM_SHADE, color);
	textmode_hline(xm + 1, y, slider_width - xm + x0, CP_MEDIUM_SHADE, color);
	textmode_putchar_color(xm, y, CP_SQUARE, color);
	textmode_putchar_color(x1, y, CP_RIGHT_TRIANGLE, color);
}

void range_process_events(ui_range_t *range, ui_event_t *event)
{
	switch (event->type) {
		case UI_EVENT_MOUSEMOVE:
		case UI_EVENT_MOUSEDOWN: {
			if (event->payload.mouse.buttons & 1 > 0) {
				rect_t clientrect = get_clientrect(&(range->bounding_rect));
				clientrect.width -= 4;
				if ((clientrect.width<3) || (clientrect.height < 1)) {
					return;
				}
				if (rect_test_mouse(&clientrect, event->payload.mouse.x, event->payload.mouse.y)) {
					uint8_t x = (uint8_t)(event->payload.mouse.x / 8);
					x = x - clientrect.x - 1;
					if (x >= 0 && x < clientrect.width - 1) {
						float inner_width = (float)(clientrect.width - 2);
						float f = (float)(x) / inner_width;
						int newValue = range->min + (int)(f * (float)(range->max - range->min + 1) + 0.5f);
						range->value = CLAMP(newValue, range->min, range->max);
						range->paint = true;
					}
				}
			}
			break;
		}
		case UI_EVENT_MOUSEUP: {
			ui_handle_mouseup((ui_generic_t *)range, event);
			break;
		}
	}
	if (event->type == UI_EVENT_KEY) {
		if (range->focused == false) {
			return;
		}
		switch (event->payload.keyboard.keyCode) {
			case KEY_ARROW_LEFT: {
				range->value = MAX(range->min, range->value - range->step);
				range->paint = true;
				break;
			}
			case KEY_ARROW_RIGHT: {
				range->value = MIN(range->max, range->value + range->step);
				range->paint = true;
				break;
			}
		}
	}
}
