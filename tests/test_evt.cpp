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

	if (g_hasMouse) {
		mouse_show();
		mouse_set_vertical_range(0, g_modeInfo->numRows * 8 - 8);
	}

	textmode_print("Hello World!", 33, 25, 0x1e);
	textmode_fill_area(1,5,4,5, '.', 0x3f);
	textmode_box(12,3,30,7,0x4f);
	textmode_dblbox(15,14,30,7,0x2e);

	while (!done) {
		poll_event(&event);
		textmode_gotoxy(1,1);
		switch (event.type) {
			case UI_EVENT_MOUSEMOVE:
				printf("MOVE %d | %d     \n",
					g_mouse.mouseX,
					g_mouse.mouseY
				);
				break;
			case UI_EVENT_MOUSEDOWN:
				printf("DOWN %d | %d     \n",
					g_mouse.mouseX,
					g_mouse.mouseY
				);
				break;
			case UI_EVENT_MOUSEUP:
				printf("UP!  %d | %d     \n",
					g_mouse.mouseX,
					g_mouse.mouseY
				);
				break;
			case UI_EVENT_KEY:
				if (event.payload.keyboard.keyCode == 27) {
					done = true;
				}
			default:
				break;
		}
	}
	getch();

	if (g_hasMouse) {
		mouse_hide();
	}

	textmode_setmode(3);
	return 0;
}
