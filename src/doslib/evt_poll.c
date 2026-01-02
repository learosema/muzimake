
#include <string.h>
#include "asmstuff.h"
#ifdef __DOS__
#include <conio.h>
#else
#include <stubs.h>
#endif
#include <ui_event.h>
#include "evt_poll.h"
#include "mouse.h"
#include "keyboard.h"

static MOUSE_STATUS lastMouseStatus = {0};
static int lastKeyboardState = 0x80;

/* Simple internal event queue for platform backends to push events into.
 * `event_poll` will drain this queue before reading native mouse/keyboard.
 */
static const int PUSH_QUEUE_SIZE = 64;
static ui_event_t push_queue[PUSH_QUEUE_SIZE];
static int push_head = 0;
static int push_tail = 0;
static int push_count = 0;

void event_init()
{
	// kbd_interrupt_init();
	mouse_init();
	mouse_set_predefined_eventhandler(EVENT_MOUSE_ALL);
}

int event_push(const ui_event_t *ev)
{
	if (!ev) return -1;
	if (push_count >= PUSH_QUEUE_SIZE) return -1;
	push_queue[push_tail] = *ev;
	push_tail = (push_tail + 1) % PUSH_QUEUE_SIZE;
	push_count++;
	return 0;
}

void event_shutdown()
{
	// kbd_interrupt_shutdown();
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

	/* First, drain any events pushed by platform backends */
	while (push_count > 0 && idx < max_events) {
		events[idx] = push_queue[push_head];
		push_head = (push_head + 1) % PUSH_QUEUE_SIZE;
		push_count--;
		idx++;
		count++;
	}
	if (idx == max_events) return count;

	/* Next, check mouse callback data as before */
	if (mouse_data->has_event)
	{
		events[idx] = event_poll_mouse(mouse_data);
		count++;
		idx++;
	}
	if (idx == max_events) {
		return count;
	}
/*
	kbd_read();
	kbd_state_t * k_state = kbd_get_state();
	if (k_state->changed) {
		event.type = (k_state->last >= 0x80) ? UI_EVENT_KEYUP : UI_EVENT_KEYDOWN;
		event.payload.keyboard.keyCode = k_state->last & 0x7f;
		events[idx] = event;
		idx++;
		count++;
	}
*/
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

void wait_for_user()
{
	while (!mouse_get_callback_data()->has_event && !(kbhit())) {
		asm_hlt();
	}
}
