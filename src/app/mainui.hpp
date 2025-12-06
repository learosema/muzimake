#pragma once
#include "cmponent.h"

class MainUI {
	public:
		MainUI();
		virtual ~MainUI();

		int Setup();
		void RenderAll(const bool renderAll = false);
		bool NeedsRepaint();
		void HandleEvents();

		inline bool IsDone() const { return m_done; }

	private:
		static bool EventHandler(uint16_t element_id, ui_event_t *event);
		ui_state_t m_ui;
		bool m_hasMouse;
		bool m_done;
		ui_event_t m_events[10];
};
