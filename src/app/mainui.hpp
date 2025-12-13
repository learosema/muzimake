#pragma once
#include <stdint.h>
#include "ui_event.h"
#include "cmponent.h"
#include "helper/list.h"

class MainUI {
public:
	MainUI();
	~MainUI();

	void RenderAll(bool renderAll = false);
	bool NeedsRepaint() const;
	void HandleEvents();

	bool IsDone() const;

	/**
	 * Lifecycle helpers
	 * - `Initialize()` prepares internal state and registers UI components. Call this
	 *   after constructing `MainUI` to transition it into the running state.
	 * - `Shutdown()` performs a graceful teardown (deregisters callbacks, disposes
	 *   components) and transitions to `State_Done`. The destructor will free any
	 *   remaining resources, but calling `Shutdown()` is recommended for predictable
	 *   ordering of callback deregistration and resource release.
	 */
	bool Initialize();
	void Shutdown();

	/*
	* Public lifecycle states. Consumers may query `IsDone()` or examine `State`
	* via other instrumentation (if exposed). Use these states to make UI code
	* easier to reason about and to implement controlled shutdown paths.
	*/
enum State {
	State_Initializing = 0,
	State_Running,
	State_Paused,
	State_Stopping,
	State_Done
};


/* Test helper (debug only): return the id of the first component if set,
* otherwise 0. Used by unit tests. Note: MainUI is single-instance; component
* ids are local (the `ID_*` constants) and are not remapped to include an
* instance index. */
unsigned DebugGetFirstComponentId() const;

/* Test helper (debug only): enqueue a click event targeting the given
* local id (one of the ID_* constants). Returns true on success. This is
* intended for unit tests to exercise the event queue and processing
* without relying on platform input plumbing. */
bool DebugEnqueueClick(uint8_t local_id);

/* Test helper: process only events already enqueued via DebugEnqueueClick()
* without calling the platform `event_poll()` API. Useful for unit tests to
* avoid blocking on platform input. */
void DebugProcessQueuedEvents();

private:
	ui_state_t m_ui;
	State state;
	linked_list_t *event_queue;

	// Instance-scoped event handler called by the C trampoline.
	bool onEvent(uint16_t element_id, ui_event_t *event);

	int CreateComponents();

	// Forwarder used by the C-style callback registry. Member so it can access
	// instance state via the `ctx` pointer (which should be a `MainUI*`).
	static bool forwarder(uint16_t element_id, ui_event_t *event, void* ctx);

	// prevent copying (old-style; compatible with older compilers)
	MainUI(const MainUI&);
	MainUI& operator=(const MainUI&);
	// Note: `forwarder` is declared above and dispatches to the instance
	// `onEvent` method.
};
