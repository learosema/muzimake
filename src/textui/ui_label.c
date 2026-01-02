#include "ui_label.h"
#include "textmode.h"

void label_render(ui_label_t *label)
{
	textmode_print(label->label, label->bounding_rect.x, label->bounding_rect.y, label->color);
}

void label_process_events(ui_label_t *label, ui_event_t * event) {
	if ((event->type & UI_EVENT_MOUSEUP) > 0) {
		ui_handle_mouseup((ui_generic_t *)label, event);
 	}
}
