#include "dosapp.hpp"

#include <stdlib.h>
#if defined(__DOS__)
#include <conio.h>
#include <dos.h>
#else
#endif
#include "log.h"
#include "mouse.h"
#include "textmode.h"
#include "ui_event.h"
#include "bulkyv5.h"
#include "vga.h"

MODEINFO * g_modeInfo;

DOSApp::DOSApp() {}

int DOSApp::Setup() {
	textmode_setmode(3);

	m_hasMouse = mouse_init();
	g_modeInfo = textmode_get_modeinfo();
	APP_LOG("Loading font.");
	textmode_init_font(font_data, 8, 0, 256);
	event_init();
	atexit(event_shutdown);
	m_mainUI.Setup();
	RenderMainScreen();
	if (m_hasMouse) {
		mouse_show();
		mouse_set_vertical_range(0, g_modeInfo->numRows * 8 - 8);
	}
	return 0;
}

void DOSApp::RenderMainScreen() {

	textmode_clear(0x1e);
	textmode_cursor(32, 0);
	APP_LOG("screen resolution: %dx%d", textmode_get_modeinfo()->numCols, textmode_get_modeinfo()->numRows);

	m_mainUI.RenderAll();

	textmode_hline(0,0, 80, ' ', 0x70);
	textmode_print("MUZIMAKE \002", 1, 0, 0x74);
}

int DOSApp::MainLoop()
{
	while (! m_mainUI.IsDone()) {
		if (m_mainUI.NeedsRepaint()) {
			vga_wait_for_retrace();
			// textmode_putchar(78,0, x++);
			if (m_hasMouse) mouse_hide();
			m_mainUI.RenderAll(false);
			if (m_hasMouse) mouse_show();
		}

		wait_for_user();
		m_mainUI.HandleEvents();

	}

	return 0;
}

DOSApp::~DOSApp() {
	if (m_hasMouse) {
		mouse_hide();
	}
	textmode_dispose();
}
