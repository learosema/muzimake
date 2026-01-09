#include "ui_common.h"

bool rect_test_point_in_bounds(const rect_t * const rect, const uint8_t x, const uint8_t y)
{
	uint8_t x0 = rect->x;
	uint8_t x1 = x0 + rect->width - 1;
	uint8_t y0 = rect->y;
	uint8_t y1 = y0 + rect->height - 1;
	return (
		x >= x0 && x <= x1 && y >= y0 && y <= y1
	);
}

bool rect_test_mouse(const rect_t * const rect, const uint16_t mouseX, const uint16_t mouseY)
{
	return rect_test_point_in_bounds(rect, (uint8_t)(mouseX>>3), (uint8_t)(mouseY>>3));
}

rect_t get_clientrect(const rect_t * const bounding_rect)
{
	rect_t clientrect = *bounding_rect;
	if (clientrect.height > 2 && clientrect.width > 2)
	{
		clientrect.x += 1;
		clientrect.y += 1;
		clientrect.width -= 2;
		clientrect.height -= 2;
	}
	return clientrect;
}
/*
void ui_handle_mouseup(ui_generic_t *generic, ui_event_t *event)
{
	if ((event->type & UI_EVENT_MOUSEUP) == 0) {
		return;
	}
	ui_event_t clickEvent = {0};
	clickEvent.type = UI_EVENT_CLICK;
	clickEvent.payload.click.target = generic->id;
	clickEvent.payload.click.buttons = event->payload.mouse.buttons;
	if (! rect_test_mouse(&(generic->bounding_rect), event->payload.mouse.x, event->payload.mouse.y))
	{
		if (generic->focused) {
			generic->focused = false;
			generic->paint = true;
		}
		return;
	}
	if (generic->event_handler != NULL) {
		bool result = generic->event_handler(generic->id, &clickEvent);
		if (! result) {
			return;
		}
	}
	generic->focused = true;
	generic->paint = true;
}
*/
