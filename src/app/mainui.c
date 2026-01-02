#include "mainui.hpp"
#include "cmponent.h"
#include "ui_common.h"
#include "ui_event.h"
#include "ui_load.h"

#include <stdint.h>
#include <stdlib.h>
#if defined __DOS__ && defined __WATCOM__
#include <dos.h>
#endif

#include "helper/list.h"

// Small fixed-size C-style callback registry to avoid dependency on std::
struct CallbackEntry {
	uint16_t id;
	bool (*fn)(uint16_t, ui_event_t*, void*);
	void* ctx;
};

static const int MAX_CALLBACKS = 32;
static CallbackEntry g_callbacks[MAX_CALLBACKS];
static int g_callback_count = 0;

// Instance allocation so we can create instance-scoped element ids.
/* Singleton support: application uses a single MainUI instance. This
 * simplifies id handling (no instance-scoped high byte) and avoids the
 * complexity of managing instance indices. */
static MainUI* g_mainui_singleton = NULL;

static void register_callback(uint16_t id, bool (*fn)(uint16_t, ui_event_t*, void*), void* ctx)
{
	int i;
	for (i = 0; i < g_callback_count; ++i) {
		if (g_callbacks[i].id == id) {
			g_callbacks[i].fn = fn;
			g_callbacks[i].ctx = ctx;
			return;
		}
	}
	if (g_callback_count < MAX_CALLBACKS) {
		g_callbacks[g_callback_count].id = id;
		g_callbacks[g_callback_count].fn = fn;
		g_callbacks[g_callback_count].ctx = ctx;
		++g_callback_count;
	}
}

static void unregister_callback(uint16_t id)
{
	int i;
	for (i = 0; i < g_callback_count; ++i) {
		if (g_callbacks[i].id == id) {
			g_callbacks[i] = g_callbacks[g_callback_count - 1];
			--g_callback_count;
			return;
		}
	}
}

static CallbackEntry* find_callback(uint16_t id)
{
	int i;
	for (i = 0; i < g_callback_count; ++i) {
		if (g_callbacks[i].id == id) return &g_callbacks[i];
	}
	return NULL;
}

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

// Trampoline used as the C-style callback; looks up the C-style registry and
// invokes it. The registered callback uses the `void* ctx` to refer to a
// `MainUI*` instance when forwarding.
extern "C" bool EventTrampoline(uint16_t element_id, ui_event_t *event)
{
	CallbackEntry* e = find_callback(element_id);
	if (e && e->fn) {
		return e->fn(element_id, event, e->ctx);
	}
	return true;
}

// MainUI internal members were moved into the public class to simplify the
// design: MainUI behaves the same across DOS and SDL backends and does not
// need an opaque PIMPL in this codebase.

// Forwarder that the C-style callback registry will call. The `ctx` pointer
// is expected to be a `MainUI*` and this method dispatches to the instance
// method `onEvent`.
bool MainUI::forwarder(uint16_t element_id, ui_event_t *event, void* ctx) {
	MainUI* self = (MainUI*)ctx;
	if (!self) return true;
	return self->onEvent(element_id, event);
}

bool MainUI::onEvent(uint16_t element_id, ui_event_t *event) {
	ui_load_result_t result = {0};
	if (event->type == UI_EVENT_CLICK) {
		/* Only handle click events that target this element. The component
		 * dispatch logic calls each component's event handler for every
		 * queued event; the handler must therefore ignore events not
		 * intended for it. Compare the event target to the element id
		 * (mask low byte for local id matching) before acting. */
		uint16_t target = event->payload.click.target & 0xff;
		uint16_t local_id = element_id & 0xff;
		if (local_id != target) return true;
		if (local_id == ID_LOAD) {
			return ui_load_modal(&result);
		}
	}
	return true;
}

MainUI::MainUI()
	: state(State_Initializing), event_queue(nullptr)
{
	m_ui.count = 0;
	m_ui.components = NULL;
	event_queue = linked_list_new();
}

MainUI::~MainUI() {
	// perform graceful shutdown if needed
	Shutdown();
	if (event_queue) {
		linked_list_dispose(event_queue);
		event_queue = NULL;
	}
}

bool MainUI::Initialize()
{
	// `this` is always available; check for previous initialization
	if (m_ui.components != NULL) return false;
	if (g_mainui_singleton != NULL) return false;
	// Setup the UI components
	if (CreateComponents() != 0) return false;
	g_mainui_singleton = this;
	state = State_Running;
	return true;
}

void MainUI::Shutdown()
{
	// If already shutting down or done, treat as no-op
	if (state == State_Stopping || state == State_Done) return;
	state = State_Stopping;
	// Deregister callbacks for owned components and dispose components
	if (m_ui.components) {
		for (uint16_t i = 0; i < m_ui.count; i++) {
			uint16_t id = m_ui.components[i].component.generic.id;
			unregister_callback(id);
		}
		for (uint16_t i = 0; i < m_ui.count; i++) {
			component_dispose(&(m_ui.components[i]));
		}
		delete [] m_ui.components;
		m_ui.count = 0;
		m_ui.components = NULL;
	}
	state = State_Done;
	if (g_mainui_singleton == this) g_mainui_singleton = NULL;
}

int MainUI::CreateComponents()
{
	m_ui.count = 6;
	m_ui.components = new ui_component_t[m_ui.count];

	m_ui.components[ID_LOAD] = component_create_button(ID_LOAD, LBL_LOAD, 1, 2, 10, 3, 0x2f);
	m_ui.components[ID_SAVE] = component_create_button(ID_SAVE, LBL_SAVE, 12, 2, 10, 3, 0x5f);
	m_ui.components[ID_PLAY] = component_create_button(ID_PLAY, LBL_PLAY, 23, 2, 10, 3, 0x3f);
	m_ui.components[ID_STOP] = component_create_button(ID_STOP, LBL_STOP, 34, 2, 10, 3, 0x1f);
	m_ui.components[ID_SHEET] = component_create_sheet(ID_SHEET, 1, 6, 78, 30, 0x0f, 32, 9);
	m_ui.components[ID_PIANO] = component_create_piano(ID_PIANO, 1, 38, 78, 8, 0x71);

	// use C trampoline and register C-style handlers (forwarders) with context
	for (uint16_t i = 0; i < m_ui.count; ++i) {
		uint16_t id = m_ui.components[i].component.generic.id;
		m_ui.components[i].component.generic.event_handler = EventTrampoline;
		/* if component has a button.event_handler field, keep it in sync */
		if (m_ui.components[i].type == COMPONENT_BUTTON) {
			m_ui.components[i].component.button.event_handler = EventTrampoline;
		}
		register_callback(id, MainUI::forwarder, this);
	}

	return 0;
}

// Note: Event handling is forwarded via the trampoline -> g_callbacks -> Impl::onEvent

void MainUI::RenderAll(bool renderAll)
{
	component_render_all(m_ui.count, m_ui.components, renderAll);
}

bool MainUI::NeedsRepaint() const {
	for (uint16_t i = 0; i < m_ui.count; i++) {
		if (m_ui.components[i].component.generic.paint) {
			return true;
		}
	}
	return false;
}

void MainUI::HandleEvents()
{
	ui_event_t buffer[16];
	uint8_t num_events = event_poll(buffer, 0, 16);

	/* enqueue polled events into dynamic linked list */
	for (uint8_t i = 0; i < num_events; ++i) {
		ui_event_t *copy = (ui_event_t *)malloc(sizeof(ui_event_t));
		if (!copy) continue;
		*copy = buffer[i];
		linked_list_append(event_queue, copy);
	}

	/* process queued events */
	node_t *node = event_queue->head;
	while (node != NULL) {
		ui_event_t *ev = (ui_event_t *)node->data;
		node_t *next = node->next; /* save next because we remove current */
		component_process_events(m_ui.count, m_ui.components, ev);
		if ((ev->type & UI_EVENT_KEY) > 0) {
			if (ev->payload.keyboard.keyCode == KEY_ALT_X) {
				state = MainUI::State_Done;
			}
		}

		linked_list_remove_node(event_queue, node);
		free(ev);

		if (state == MainUI::State_Done) break;
		node = next;
	}
}

void MainUI::DebugProcessQueuedEvents()
{
	if (m_ui.count == 0 || m_ui.components == NULL) return;
	/* process queued events only (do not call event_poll) */
	node_t *node = event_queue->head;
	while (node != NULL) {
		ui_event_t *ev = (ui_event_t *)node->data;
		node_t *next = node->next; /* save next because we remove current */
		component_process_events(m_ui.count, m_ui.components, ev);
		if ((ev->type & UI_EVENT_KEY) > 0) {
			if (ev->payload.keyboard.keyCode == KEY_ALT_X) {
				state = MainUI::State_Done;
			}
		}

		linked_list_remove_node(event_queue, node);
		free(ev);

		if (state == MainUI::State_Done) break;
		node = next;
	}
}

bool MainUI::IsDone() const {
	return state == State_Done;
}

unsigned MainUI::DebugGetFirstComponentId() const
{
	if (m_ui.count == 0 || m_ui.components == NULL) return 0;
	return (unsigned)m_ui.components[0].component.generic.id;
}

bool MainUI::DebugEnqueueClick(uint8_t local_id)
{
	if (m_ui.count == 0 || m_ui.components == NULL) return false;
	if (local_id >= m_ui.count) return false;

	ui_event_t *copy = (ui_event_t *)malloc(sizeof(ui_event_t));
	if (!copy) return false;
	copy->type = UI_EVENT_CLICK;
	/* target must be the remapped instance-scoped id */
	copy->payload.click.buttons = 1;

	copy->payload.click.target = m_ui.components[local_id].component.generic.id;
	linked_list_append(event_queue, copy);
	return true;
}
