#include "mainui.hpp"

#include "cmponent.h"
#include "ui_common.h"
#include "ui_event.h"
#include "ui_load.h"

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

MainUI::MainUI() {
	m_done = false;
}

MainUI::~MainUI()
{
	for (uint16_t i = 0; i < m_ui.count; i++)
	{
		component_dispose(&(m_ui.components[i]));
	}
	delete (m_ui.components);
	m_ui.count = 0;
	m_ui.components = nullptr;
}

int MainUI::Setup()
{
	m_ui.count = 6;
	m_ui.components = new ui_component_t[m_ui.count];

	m_ui.components[ID_LOAD] = component_create_button(ID_LOAD, LBL_LOAD, 1, 2, 10, 3, 0x2f);
	m_ui.components[ID_SAVE] = component_create_button(ID_SAVE, LBL_SAVE, 12, 2, 10, 3, 0x5f);
	m_ui.components[ID_PLAY] = component_create_button(ID_PLAY, LBL_PLAY, 23, 2, 10, 3, 0x3f);
	m_ui.components[ID_STOP] = component_create_button(ID_STOP, LBL_STOP, 34, 2, 10, 3, 0x1f);
	m_ui.components[ID_SHEET] = component_create_sheet(ID_SHEET, 1, 6, 78, 30, 0x0f, 32, 9);
	m_ui.components[ID_PIANO] = component_create_piano(ID_PIANO, 1, 38, 78, 8, 0x71);

	m_ui.components[ID_LOAD].component.button.event_handler = MainUI::EventHandler;
	m_ui.components[ID_SAVE].component.button.event_handler = MainUI::EventHandler;
	m_ui.components[ID_PLAY].component.button.event_handler = MainUI::EventHandler;
	m_ui.components[ID_STOP].component.button.event_handler = MainUI::EventHandler;

	return 0;
}

bool MainUI::EventHandler(uint16_t element_id, ui_event_t *event)
{
	ui_load_result_t result = {0};
	if (event->type == UI_EVENT_CLICK) {
		if (element_id == ID_LOAD) {
			return ui_load_modal(&result);
		}
	}
	return true;
}

void MainUI::RenderAll(const bool renderAll)
{
	component_render_all(m_ui.count, m_ui.components, renderAll);
}

bool MainUI::NeedsRepaint() {
	for (uint16_t i = 0; i < m_ui.count; i++)
	{
		if (m_ui.components[i].component.generic.paint) {
			return true;
		}
	}
	return false;
}

void MainUI::HandleEvents()
{
	uint8_t num_events = event_poll(m_events, 0, 10);
	for (uint8_t event_idx = 0; event_idx < num_events; event_idx++) {
		component_process_events(m_ui.count, m_ui.components, &(m_events[event_idx]));
		if ((m_events[event_idx].type & UI_EVENT_KEY) > 0) {
			if (m_events[event_idx].payload.keyboard.keyCode == KEY_ALT_X) {
				// press alt+x to quit
				m_done = true;
				break;
			}
		}
	}
}
