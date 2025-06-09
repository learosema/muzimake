#include <stdlib.h>
#include <string.h>
#ifdef __DOS__
#include <conio.h>
#else
#include <stubs.h>
#endif
#include "ui_event.h"
#include "mouse.h"
#include "keyboard.h"

static MOUSE_STATUS lastMouseStatus = {0};
static int lastKeyboardState = 0x80;

void event_init()
{
	kbd_interrupt_init();
	mouse_init();
	mouse_set_predefined_eventhandler(EVENT_MOUSE_ALL);
}

void event_shutdown()
{
	kbd_interrupt_shutdown();
	mouse_init();
}

ui_event_t event_poll_mouse(mouse_callback_data_t *mouse)
{
	ui_event_t event = {0};

	if (mouse->code > 0) {
		mouse->has_event = false;
		lastMouseStatus.buttons = mouse->button_state;
		event.type = mouse->code;
		event.payload.mouse.x = mouse->x_pos;
		event.payload.mouse.y = mouse->y_pos;
		event.payload.mouse.deltaX = mouse->x_counts;
		event.payload.mouse.deltaY = mouse->y_counts;
		event.payload.mouse.buttons = mouse->button_state;
	}
	return event;
}


uint8_t event_poll(ui_event_t *events, uint16_t offset, uint16_t max_events)
{
	uint16_t count = 0;
	uint16_t idx = offset;
	ui_event_t event = {0};

	MOUSE_STATUS mouseStatus = {0};
	mouse_callback_data_t *mouse_data = mouse_get_callback_data();

	if (idx == max_events) {
		return 0;
	}

	if (mouse_data->has_event)
	{
		events[idx] = event_poll_mouse(mouse_data);
		count++;
		idx++;
	}
	if (idx == max_events) {
		return count;
	}

	if (!kbd_get_state()->has_event) {
		kbd_get_state()->has_event = false;
	}

	if (!kbhit()) {
		return count;
	}

	uint16_t ch = kbd_getkey_int16();

	event.type = UI_EVENT_KEY;
	event.payload.keyboard.keyCode = ch;
	events[idx] = event;
	count++;

	kbd_clear_buffer();

	return count;
}

void event_clear(ui_event_t *event)
{
	memset(event,0, sizeof(ui_event_t));
}
