#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__DOS__)
#include <conio.h>
#include <dos.h>
#else
#include <stubs.h>
#endif
#include "assembla.h"
#include "textui/mouse.h"
#include "textui/keyboard.h"
#include "textui/textmode.h"
#include "textui/ui_event.h"
#include "textui/cmponent.h"
#include "macros.h"
#include "helper/log.h"
#include "fonts/bulkyv5.h"
#include "vga.h"

static const char * LBL_OK = "Okay";
static const char * LBL_CANCEL = "Cancel";
static const char * LBL_PLAY = "Play";
static const char * LBL_STOP = "Stop";
static const char * LBL_FILENAME = "Filename";

static const char *LIST_EXAMPLE[30] = {
	"Piano", "Violin", "BaseDrum", "Beep", "Boop",
	"Sawtooth", "Triangle", "Square", "Sine", "Pulse",
	"Snare", "Xylophone", "Dreizehn", "Vierzehn", "Fuenfzehn",
	"Sechzehn", "Siebzehn", "Achtzehn", "Neunzehn",
	"Zwanzig", "Einundzwanzig", "Zweiundzwanzig", "Dreiundzwanzig",
	"Vierundzwanzig", "Fuenfundzwanzig", "Sechsundzwanzig", "Siebenundzwanzig",
	"Achtundzwanzig", "Neunundzwanzig", "Dreissig"
};

#define ID_OK         0
#define ID_CANCEL     1
#define ID_INPUTLABEL 2
#define ID_INPUT      3
#define ID_LIST       4
#define ID_RANGE      5
#define ID_SHEET			6
#define ID_PIANO			7

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

	ui.count = 8;
	ui.components = ALLOC_TYPE(ui_component_t, ui.count);

	ui.components[ID_OK] = component_create_button(ID_OK, LBL_OK, 2, 2, 10, 3, 0x2f);;
	ui.components[ID_CANCEL] = component_create_button(ID_CANCEL, LBL_CANCEL, 13, 2, 10, 3, 0x4e);
	ui.components[ID_INPUTLABEL] = component_create_label(ID_INPUTLABEL, ID_INPUT, LBL_FILENAME, 3, 7, 0x1f);
	ui.components[ID_INPUT] = component_create_input(ID_INPUT, 2, 8, 40, 3, 0x5f, "", 80);
	ui.components[ID_LIST] = component_create_listbox(ID_LIST, 2, 12, 12,10, 0x6e, LIST_EXAMPLE, 30);
	ui.components[ID_RANGE] = component_create_range(ID_RANGE, 16, 12, 22, 3, 0x3f, 20, 10, 30, 1);
	ui.components[ID_SHEET] = component_create_sheet(ID_SHEET, 1, 24, 78, 12, 0x0f, 32, 9);
	ui.components[ID_PIANO] = component_create_piano(ID_PIANO, 1, 38, 78, 8, 0x71);

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
	while (!mouse_get_callback_data()->has_event && !kbhit()) {
		asm_hlt();
	}
}

int main()
{
	ui_event_t events[10] = {0};
	bool done = false;
	textmode_setmode(3);
	g_hasMouse = mouse_init();
	textmode_font8();
	g_modeInfo = textmode_get_modeinfo();
	textmode_init_font(font_data, 8, 0, 256);
	APP_LOG("TextUI-Tests started.");
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
	textmode_print("MUZIMAKE UI Test", 1, 0, 0x74);

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
				textmode_gotoxy(1, 47);
				printf("KEY     %04x\n", events[event_idx].payload.keyboard.keyCode);
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
	textmode_setmode(3);
	APP_LOG("Text-UI tests finished.");
	return 0;
}
