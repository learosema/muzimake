#include <stdlib.h>
#include <string.h>
#ifdef __DOS__
#include <conio.h>
#else
#include <stubs.h>
#endif
#include "ui_event.h"
#include "mouse.h"

static MOUSE_STATUS lastMouseStatus = {0};
static int lastKeyboardState = 0x80;

void event_poll(ui_event_t *result)
{
	ui_event_t event = {0};

	MOUSE_STATUS mouseStatus = {0};

	mouse_get_status(&mouseStatus);
	if ((mouseStatus.buttons > 0) && (lastMouseStatus.buttons == 0))
	{
		lastMouseStatus.buttons = mouseStatus.buttons;
		event.type = UI_EVENT_MOUSEDOWN;
		event.payload.mouse.x = mouseStatus.mouseX;
		event.payload.mouse.y = mouseStatus.mouseY;
		event.payload.mouse.buttons = mouseStatus.buttons;
		*result = event;
		return;
	}

	if ((mouseStatus.buttons == 0) && (lastMouseStatus.buttons > 0))
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
		*result = event;
		return;
	}
/** ' clear keyboard buffer
DEF SEG = &H40
POKE &H1A, PEEK(&H1C) */
/*
	int keyState = inp(0x60);
	if (keyState & 0x80 > 0 && lastKeyboardState < 0x80) {
		lastKeyboardState = keyState;
		event.type = UI_EVENT_KEYUP;
		event.payload.keyboard.keyCode = keyState & 0x7f;
		*result = event;
		return;
	}

	if (keyState & 0x80 == 0 && lastKeyboardState >= 0x80) {
		lastKeyboardState = keyState;
		event.type = UI_EVENT_KEYDOWN;
		event.payload.keyboard.keyCode = keyState & 0x7f;
		*result = event;
		return;
	}
*/
	if (kbhit()) {
		uint16_t ch = getch();
		if (ch == 0) {
			ch = getch();
			ch <<= 8;
		}
		event.type = UI_EVENT_KEY;
		event.payload.keyboard.keyCode = ch;
		*result = event;
		return;
	}

	event_clear(result);
}

void event_clear(ui_event_t *event)
{
	memset(event,0, sizeof(ui_event_t));
}
