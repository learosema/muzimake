#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__DOS__)
#include <conio.h>
#include <dos.h>
#else
#include <stubs.h>
#endif
#include "asmstuff.h"
#include "textui/mouse.h"
#include "textui/keyboard.h"
#include "textui/textmode.h"
#include "textui/ui_event.h"
#include "textui/cmponent.h"
#include "macros.h"
#include "helper/log.h"
#include "fonts/bulkyv5.h"
#include "vga.h"

static const char * LBL_LOAD = "Load";
static const char * LBL_SAVE = "Save";
static const char * LBL_PLAY = "Play";
static const char * LBL_STOP = "Stop";
static const char * LBL_FILENAME = "Filename";

#define ID_LOAD			0
#define ID_SAVE			1
#define ID_PLAY			2
#define ID_STOP			3
#define ID_SHEET		4
#define ID_PIANO		5

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

	ui.count = 6;
	ui.components = ALLOC_TYPE(ui_component_t, ui.count);

	ui.components[ID_LOAD] = component_create_button(ID_LOAD, LBL_LOAD, 1, 2, 10, 3, 0x2f);
	ui.components[ID_SAVE] = component_create_button(ID_SAVE, LBL_SAVE, 12, 2, 10, 3, 0x5f);
	ui.components[ID_PLAY] = component_create_button(ID_PLAY, LBL_PLAY, 23, 2, 10, 3, 0x3f);
	ui.components[ID_STOP] = component_create_button(ID_STOP, LBL_STOP, 34, 2, 10, 3, 0x1f);
	ui.components[ID_SHEET] = component_create_sheet(ID_SHEET, 1, 6, 78, 30, 0x0f, 32, 9);
	ui.components[ID_PIANO] = component_create_piano(ID_PIANO, 1, 38, 78, 8, 0x71);

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

bool needs_repaint(const ui_state_t *ui) {
	for (uint16_t i = 0; i < ui->count; i++)
	{
		if (ui->components[i].component.generic.paint) {
			return true;
		}
	}
	return false;
}

void wait_for_user()
{
	while (!mouse_get_callback_data()->has_event && !(kbhit())) {
		asm_hlt();
	}
}

int main()
{
	APP_LOG("TextUI-Tests started.");
	ui_event_t events[10] = {0};
	bool done = false;
	textmode_setmode(3);
	g_hasMouse = mouse_init();
	g_modeInfo = textmode_get_modeinfo();
	APP_LOG("Loading font.");
	textmode_init_font(font_data, 8, 0, 256);
	event_init();
	atexit(event_shutdown);

	textmode_clear(0x1e);
	textmode_cursor(32, 0);

	ui_state_t ui = ui_create();

	component_render_all(ui.count, ui.components, true);

	if (g_hasMouse) {
		mouse_show();
		mouse_set_vertical_range(0, g_modeInfo->numRows * 8 - 8);
	}

	textmode_hline(0,0, 80, ' ', 0x70);
	textmode_print("MUZIMAKE \001", 1, 0, 0x74);

	while (!done) {
		if (needs_repaint(&ui)) {
			vga_wait_for_retrace();
			// textmode_putchar(78,0, x++);
			if (g_hasMouse) mouse_hide();
			component_render_all(ui.count, ui.components, false);
			if (g_hasMouse) mouse_show();
		}
		wait_for_user();

		uint8_t num_events = event_poll(events, 0, 10);
		for (uint8_t event_idx = 0; event_idx < num_events; event_idx++) {
			component_process_events(ui.count, ui.components, &(events[event_idx]));
			if ((events[event_idx].type & UI_EVENT_KEY) > 0) {
				if (events[event_idx].payload.keyboard.keyCode == KEY_ALT_X) {
					// press alt+x to quit
					done = true;
					break;
				}
			}
		}
	}

	if (g_hasMouse) {
		mouse_hide();
	}
	ui_cleanup(&ui);
	textmode_dispose();
	APP_LOG("Text-UI tests finished.");
	return 0;
}
