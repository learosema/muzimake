#include <stdlib.h>
#ifdef __DOS__
#include <conio.h>
#else
#include <stubs.h>
#endif
#include "ui_event.h"
#include "mouse.h"

static MOUSE_STATUS lastMouseStatus = {0};

void poll_event(ui_event_t *result)
{
	ui_event_t event = {0};

	MOUSE_STATUS mouseStatus = {0};

	mouse_get_status(&mouseStatus);
	if ((mouseStatus.buttons & 1 > 0) && (lastMouseStatus.buttons & 1 == 0))
	{
		lastMouseStatus.buttons = mouseStatus.buttons;
		event.type = UI_EVENT_MOUSEDOWN;
		event.payload.mouse.x = mouseStatus.mouseX;
		event.payload.mouse.y = mouseStatus.mouseY;
		event.payload.mouse.buttons = mouseStatus.buttons;
		*result = event;
		return;
	}

	if ((mouseStatus.buttons & 1 > 0) && (lastMouseStatus.buttons & 1 == 0))
	{
		lastMouseStatus.buttons = mouseStatus.buttons;
		event.type = UI_EVENT_MOUSEUP;
		event.payload.mouse.x = mouseStatus.mouseX;
		event.payload.mouse.y = mouseStatus.mouseY;
		event.payload.mouse.buttons = mouseStatus.buttons;
		*result = event;
		return;
	}

	if ((mouseStatus.mouseX != lastMouseStatus.mouseX) || (mouseStatus.mouseY != lastMouseStatus.mouseY)) {
		lastMouseStatus.mouseX = mouseStatus.mouseX;
		lastMouseStatus.mouseY = mouseStatus.mouseY;
		event.type = UI_EVENT_MOUSEMOVE;
		event.payload.mouse.x = mouseStatus.mouseX;
		event.payload.mouse.y = mouseStatus.mouseY;
		event.payload.mouse.buttons = mouseStatus.buttons;
		return;
	}

	if ((kbhit())) {
		event.type = UI_EVENT_KEY;
		event.payload.keyboard.keyCode = getch();
		return;
	}
}
