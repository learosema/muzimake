#pragma once
#include "iapp.hpp"
#include <stdint.h>

class SDLApp : public IApp {

public:
	SDLApp();
	virtual ~SDLApp();

	int Setup();
	int MainLoop();
private:
    /* Opaque handles to avoid requiring SDL headers in the public header. */
    void *sdl_window_;
    void *sdl_renderer_;
    bool sdl_initialized_;
	void *sdl_texture_;
	/* Helper methods refactored out of MainLoop for clarity. Implementations
	 * live in `sdlapp.cpp` and avoid SDL types in the public header. */
	bool EnsureTexture(int tex_w, int tex_h);
	void RenderBufferToTexture(const uint8_t *buffer, int cols, int rows, int glyph_w, int glyph_h);
	void ProcessSDLEvents(int cols, int rows, int glyph_w, int glyph_h, int dst_x, int dst_y, int scale, bool &running);
	void UpdateKeyRepeats();

	struct KeyState {
		int32_t keycode; /* SDL keycode stored as int32 to avoid SDL types in header */
		bool used;
		bool down;
		uint32_t next_repeat_ms;
	};

	static const int MAX_TRACKED_KEYS = 32;
	KeyState key_states_[MAX_TRACKED_KEYS];
	const uint32_t KEY_REPEAT_DELAY_MS = 200;
	const uint32_t KEY_REPEAT_INTERVAL_MS = 50;
};
