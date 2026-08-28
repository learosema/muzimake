#include "ui_common.h"


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
