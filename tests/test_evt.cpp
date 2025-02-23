#include <stdio.h>
#include <stdlib.h>
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
#include "macros.h"

static const char * LBL_OK = "Okay";
static const char * LBL_CANCEL = "Cancel";

#define ID_OK     0
#define ID_CANCEL 1
#define ID_INPUT  2

bool g_hasMouse;
MOUSE_STATUS g_mouse;
MODEINFO * g_modeInfo;

static bool event_handler(uint16_t elementId, ui_event_t *event) {
	// Event :)
	if (event->type == UI_EVENT_CLICK) {
		textmode_gotoxy(15,15);
		textmode_print("CLICK!", 1,45, 0x2f);
	}
	return true;
}

ui_state_t ui_create() {
	ui_state_t ui = {0};

	ui.count = 3;
	ui.components = ALLOC_TYPE(ui_component_t, ui.count);

	ui.components[ID_OK] = component_create_button(ID_OK, LBL_OK, 2, 2, 10, 3, 0x2f);;
	ui.components[ID_CANCEL] = component_create_button(ID_CANCEL, LBL_CANCEL, 13, 2, 10, 3, 0x4e);
	ui.components[ID_INPUT] = component_create_input(ID_INPUT, 2, 5, 10, 3, 0x5f, "", 10);
	ui.components[ID_OK].component.button.event_handler = event_handler;
	ui.components[ID_CANCEL].component.button.event_handler = event_handler;
	return ui;
}

void ui_cleanup(ui_state_t *ui) {
	for (uint16_t i = 0; i < ui->count; i++)
	{
		component_dispose(&(ui->components[i]));
	}
	free(ui->components);
	ui->count = 0;
	ui->components = nullptr;
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

	ui_state_t ui = ui_create();

	component_render_all(ui.count, ui.components, true);
	if (g_hasMouse) {
		mouse_show();
		mouse_set_vertical_range(0, g_modeInfo->numRows * 8 - 8);
	}

	textmode_print("Hello World!", 33, 25, 0x1e);

	while (!done) {
		if (g_hasMouse) mouse_hide();
		component_render_all(ui.count, ui.components, true);
		if (g_hasMouse) mouse_show();
		event_poll(&event);
		component_process_events(ui.count, ui.components, &event);
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
				textmode_gotoxy(1, 47);
				printf("KEY     %x      \n", event.payload.keyboard.keyCode);
				if (event.payload.keyboard.keyCode == KEY_ALT_X) {
					// press alt+x to quit
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
	ui_cleanup(&ui);
	textmode_setmode(3);
	return 0;
}
