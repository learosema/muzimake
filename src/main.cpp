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

bool g_hasMouse;
MOUSE_STATUS g_mouse;
MODEINFO * g_modeInfo;






int main()
{
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

	while (!kbhit()) {
		textmode_gotoxy(1,1);
		mouse_get_status(&g_mouse);
		printf("%d, %d | %d | %d     \n",
			g_modeInfo->numRows,
			g_mouse.mouseX,
			g_mouse.mouseY,
			g_mouse.buttons
		);
	}
	getch();

	if (g_hasMouse) {
		mouse_hide();
	}

	textmode_setmode(3);
	return 0;
}
