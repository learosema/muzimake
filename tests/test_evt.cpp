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
#include "textui/component.h"

static char *test="Muh!";

bool g_hasMouse;
MOUSE_STATUS g_mouse;
MODEINFO * g_modeInfo;

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

	uint8_t componentCount = 3;
	ui_component_t components[3];
	components[0] = component_create_button(1, "Okay", 2, 2, 10, 3, 0x2f);;
	components[1] = component_create_button(2, "Cancel", 13, 2, 10, 3, 0x4e);
	components[2] = component_create_input(2, "Muh", 2, 5, 10, 3, 0x5f, "", 10);

	for (uint8_t i = 0; i < 3; i++) {
		component_render(&(components[i]));
	}
	if (g_hasMouse) {
		mouse_show();
		mouse_set_vertical_range(0, g_modeInfo->numRows * 8 - 8);
	}

	textmode_print("Hello World!", 33, 25, 0x1e);

	while (!done) {
		poll_event(&event);
		component_process_events(componentCount, components, &event);
		textmode_gotoxy(1, 48);
		switch (event.type) {
			case UI_EVENT_MOUSEMOVE:
				printf("MOVE %d | %d     \n",
					event.payload.mouse.x,
					event.payload.mouse.y
				);
				break;
			case UI_EVENT_MOUSEDOWN:
				printf("DOWN %d | %d     \n",
					event.payload.mouse.x,
					event.payload.mouse.y
				);
				break;
			case UI_EVENT_MOUSEUP:
				printf("UP!  %d | %d     \n",
					event.payload.mouse.x,
					event.payload.mouse.y
				);
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
