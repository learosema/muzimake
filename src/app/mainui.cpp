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

// Trampoline used as the C-style callback; looks up the C-style registry and invokes it
extern "C" bool EventTrampoline(uint16_t element_id, ui_event_t *event)
{
	CallbackEntry* e = find_callback(element_id);
	if (e && e->fn) {
		return e->fn(element_id, event, e->ctx);
	}
	return true;
}

struct MainUI::Impl {
	ui_state_t m_ui;
	MainUI::State state;
	linked_list_t *event_queue;

	Impl() : state(MainUI::State_Initializing), event_queue(NULL) {
		m_ui.count = 0;
		m_ui.components = NULL;
		event_queue = linked_list_new();
	}

	void shutdown()
	{
		if (m_ui.components) {
			// deregister callbacks for owned components
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
	}

	~Impl() {
		shutdown();
		if (event_queue) {
			linked_list_dispose(event_queue);
			event_queue = NULL;
		}
	}

	bool onEvent(uint16_t element_id, ui_event_t *event) {
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
};

// Now that MainUI::Impl is defined, implement the forwarder.
bool MainUI::impl_forwarder(uint16_t element_id, ui_event_t *event, void* ctx) {
	MainUI::Impl* impl = (MainUI::Impl*)ctx;
	if (!impl) return true;
	return impl->onEvent(element_id, event);
}

MainUI::MainUI() : impl_(new Impl()) {}

MainUI::~MainUI() {
	delete impl_;
}

bool MainUI::Initialize()
{
	if (!impl_) return false;
	if (g_mainui_singleton != NULL) return false;
	// Setup the UI components
	if (Setup() != 0) return false;
	g_mainui_singleton = this;
	impl_->state = State_Running;
	return true;
}

void MainUI::Shutdown()
{
	if (!impl_) return;
	impl_->state = State_Stopping;
	impl_->shutdown();
	impl_->state = State_Done;
	if (g_mainui_singleton == this) g_mainui_singleton = NULL;
}

int MainUI::Setup()
{
	impl_->m_ui.count = 6;
	impl_->m_ui.components = new ui_component_t[impl_->m_ui.count];

	/* Single-instance mode: use local ids directly (no high-byte remapping). */
	uint16_t instance_base = 0;

	impl_->m_ui.components[ID_LOAD] = component_create_button(ID_LOAD, LBL_LOAD, 1, 2, 10, 3, 0x2f);
	impl_->m_ui.components[ID_SAVE] = component_create_button(ID_SAVE, LBL_SAVE, 12, 2, 10, 3, 0x5f);
	impl_->m_ui.components[ID_PLAY] = component_create_button(ID_PLAY, LBL_PLAY, 23, 2, 10, 3, 0x3f);
	impl_->m_ui.components[ID_STOP] = component_create_button(ID_STOP, LBL_STOP, 34, 2, 10, 3, 0x1f);
	impl_->m_ui.components[ID_SHEET] = component_create_sheet(ID_SHEET, 1, 6, 78, 30, 0x0f, 32, 9);
	impl_->m_ui.components[ID_PIANO] = component_create_piano(ID_PIANO, 1, 38, 78, 8, 0x71);

	/* remap component ids to instance-scoped ids (high byte = instance index) */
	for (uint16_t i = 0; i < impl_->m_ui.count; ++i) {
		uint16_t local_id = impl_->m_ui.components[i].component.generic.id & 0xff;
		uint16_t new_id = (uint16_t)(instance_base | local_id);
		impl_->m_ui.components[i].component.generic.id = new_id;
		/* also update the type-specific id fields where present */
		switch (impl_->m_ui.components[i].type) {
			case COMPONENT_BUTTON:
				impl_->m_ui.components[i].component.button.id = new_id;
				break;
			case COMPONENT_LABEL:
				impl_->m_ui.components[i].component.label.id = new_id;
				break;
			case COMPONENT_INPUT:
				impl_->m_ui.components[i].component.input.id = new_id;
				break;
			case COMPONENT_LISTBOX:
				impl_->m_ui.components[i].component.listbox.id = new_id;
				break;
			case COMPONENT_RANGE:
				impl_->m_ui.components[i].component.range.id = new_id;
				break;
			case COMPONENT_PIANO:
				impl_->m_ui.components[i].component.piano.id = new_id;
				break;
			case COMPONENT_SHEET:
				impl_->m_ui.components[i].component.sheet.id = new_id;
				break;
		}
	}

	// use C trampoline and register C-style handlers (forwarders) with context
	for (uint16_t i = 0; i < impl_->m_ui.count; ++i) {
		uint16_t id = impl_->m_ui.components[i].component.generic.id;
		impl_->m_ui.components[i].component.generic.event_handler = EventTrampoline;
		/* if component has a button.event_handler field, keep it in sync */
		if (impl_->m_ui.components[i].type == COMPONENT_BUTTON) {
			impl_->m_ui.components[i].component.button.event_handler = EventTrampoline;
		}
		register_callback(id, MainUI::impl_forwarder, impl_);
	}

	return 0;
}

// Note: Event handling is forwarded via the trampoline -> g_callbacks -> Impl::onEvent

void MainUI::RenderAll(bool renderAll)
{
	component_render_all(impl_->m_ui.count, impl_->m_ui.components, renderAll);
}

bool MainUI::NeedsRepaint() const {
	for (uint16_t i = 0; i < impl_->m_ui.count; i++) {
		if (impl_->m_ui.components[i].component.generic.paint) {
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
		linked_list_append(impl_->event_queue, copy);
	}

	/* process queued events */
	node_t *node = impl_->event_queue->head;
	while (node != NULL) {
		ui_event_t *ev = (ui_event_t *)node->data;
		node_t *next = node->next; /* save next because we remove current */

		component_process_events(impl_->m_ui.count, impl_->m_ui.components, ev);
		if ((ev->type & UI_EVENT_KEY) > 0) {
			if (ev->payload.keyboard.keyCode == KEY_ALT_X) {
				impl_->state = MainUI::State_Done;
			}
		}

		linked_list_remove_node(impl_->event_queue, node);
		free(ev);

		if (impl_->state == MainUI::State_Done) break;
		node = next;
	}
}

void MainUI::DebugProcessQueuedEvents()
{
	if (!impl_) return;
	/* process queued events only (do not call event_poll) */
	node_t *node = impl_->event_queue->head;
	while (node != NULL) {
		ui_event_t *ev = (ui_event_t *)node->data;
		node_t *next = node->next; /* save next because we remove current */

		component_process_events(impl_->m_ui.count, impl_->m_ui.components, ev);
		if ((ev->type & UI_EVENT_KEY) > 0) {
			if (ev->payload.keyboard.keyCode == KEY_ALT_X) {
				impl_->state = MainUI::State_Done;
			}
		}

		linked_list_remove_node(impl_->event_queue, node);
		free(ev);

		if (impl_->state == MainUI::State_Done) break;
		node = next;
	}
}

bool MainUI::IsDone() const {
	return impl_->state == State_Done;
}

unsigned MainUI::DebugGetFirstComponentId() const
{
	if (!impl_) return 0;
	if (impl_->m_ui.count == 0 || impl_->m_ui.components == NULL) return 0;
	return (unsigned)impl_->m_ui.components[0].component.generic.id;
}

bool MainUI::DebugEnqueueClick(uint8_t local_id)
{
	if (!impl_) return false;
	if (impl_->m_ui.count == 0 || impl_->m_ui.components == NULL) return false;
	if (local_id >= impl_->m_ui.count) return false;

	ui_event_t *copy = (ui_event_t *)malloc(sizeof(ui_event_t));
	if (!copy) return false;
	copy->type = UI_EVENT_CLICK;
	/* target must be the remapped instance-scoped id */
	copy->payload.click.target = impl_->m_ui.components[local_id].component.generic.id;
	copy->payload.click.buttons = 1;

	linked_list_append(impl_->event_queue, copy);
	return true;
}
