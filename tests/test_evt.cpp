#include <stdio.h>
#include <string.h>
#if defined(__DOS__)
#include <conio.h>
#include <dos.h>
#else
#include <stubs.h>
#endif
#include "textui/mouse.h"
#include "textui/textmode.h"
#include "textui/ui_event.h"
#include "textui/button.h"

bool g_hasMouse;
MOUSE_STATUS g_mouse;
MODEINFO * g_modeInfo;

void button_process_events(uint8_t buttonCount, ui_button_t *buttons, ui_event_t *event, ui_event_t *buttonEvent)
{
	for (uint8_t i = 0; i < buttonCount; i++)
	{
		ui_button_t button = buttons[i];
		switch (event->type) {
			case UI_EVENT_MOUSEMOVE:
				if (button.active) {
					button_render(&button);
				}
				break;
			case UI_EVENT_MOUSEDOWN:
				if (button_test_mouse(&button, event->payload.mouse.x, event->payload.mouse.y)) {
					button.active = true;
					button_render(&button);
				}
				break;
			case UI_EVENT_MOUSEUP:
				if (button.active) {
					button.active = false;
					button_render(&button);
					if (button_test_mouse(&button, event->payload.mouse.x, event->payload.mouse.y)) {
						// emitEvent('click', button);
					}
				}
				break;
			case UI_EVENT_KEY:
				// todo: shortcuts?
				break;
			default:
				break;
		}
	}
}





int main()
{
	ui_event_t event;
	bool done = false;
	textmode_setmode(3);
	g_hasMouse = mouse_init();
	textmode_font8();
	g_modeInfo = textmode_get_modeinfo();

	textmode_clear(0x1e);
	textmode_cursor(32, 0);

	ui_button_t okButton = button_create("Okay", 2, 2, 10, 3, 0x2f);
	ui_button_t cancelButton = button_create("Cancel", 13, 2, 10, 3, 0x4e);
	button_render(&okButton);
	button_render(&cancelButton);


	if (g_hasMouse) {
		mouse_show();
		mouse_set_vertical_range(0, g_modeInfo->numRows * 8 - 8);
	}

	textmode_print("Hello World!", 33, 25, 0x1e);

	while (!done) {
		poll_event(&event);
		textmode_gotoxy(1,48);
		switch (event.type) {
			case UI_EVENT_MOUSEMOVE:
				printf("MOVE %d | %d     \n",
					event.payload.mouse.x,
					event.payload.mouse.y
				);
				if (okButton.active) {
					button_render(&okButton);
				}
				if (cancelButton.active) {
					button_render(&cancelButton);
				}
				break;
			case UI_EVENT_MOUSEDOWN:
				printf("DOWN %d | %d     \n",
					event.payload.mouse.x,
					event.payload.mouse.y
				);
				if (button_test_mouse(&okButton, event.payload.mouse.x, event.payload.mouse.y)) {
					okButton.active = true;
					button_render(&okButton);
				}
				if (button_test_mouse(&cancelButton, event.payload.mouse.x, event.payload.mouse.y)) {
					cancelButton.active = true;
					button_render(&cancelButton);
				}
				break;
			case UI_EVENT_MOUSEUP:
				printf("UP!  %d | %d     \n",
					event.payload.mouse.x,
					event.payload.mouse.y
				);
				if (okButton.active) {
					okButton.active = false;
					button_render(&okButton);
				}
				if (cancelButton.active) {
					cancelButton.active = false;
					button_render(&cancelButton);
				}
				break;
			case UI_EVENT_KEYDOWN:
				printf("KEYDOWN %d      \n", event.payload.keyboard.keyCode);
				break;
			case UI_EVENT_KEYUP:
				printf("KEYUP   %d      \n", event.payload.keyboard.keyCode);
				break;
			case UI_EVENT_KEY:
				// printf("KEY     %d      \n", event.payload.keyboard.keyCode);
				if (event.payload.keyboard.keyCode == 27) {
					done = true;
				}
				break;
			default:
				break;
		}
		delay(16); // 60FPS
	}

	if (g_hasMouse) {
		mouse_hide();
	}

	textmode_setmode(3);
	return 0;
}
