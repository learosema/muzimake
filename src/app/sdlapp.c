#include "sdlapp.hpp"
#include "log.h"
#include "mainui.hpp"
#include "textmode.h"
#include "bulkyv5.h"
#include "ui_event.h"

#if defined(__has_include)
#  if __has_include(<SDL2/SDL.h>)
#    include <SDL2/SDL.h>
#    define MUZI_HAVE_SDL 1
#  elif __has_include(<SDL.h>)
#    include <SDL.h>
#    define MUZI_HAVE_SDL 1
#  else
#    define MUZI_HAVE_SDL 0
#  endif
#else
#  define MUZI_HAVE_SDL 0
#endif

SDLApp::SDLApp()
	: sdl_window_(NULL), sdl_renderer_(NULL), sdl_initialized_(false), sdl_texture_(NULL)
{
	for (int i = 0; i < MAX_TRACKED_KEYS; ++i) {
		key_states_[i].used = false;
		key_states_[i].down = false;
		key_states_[i].keycode = 0;
		key_states_[i].next_repeat_ms = 50;
	}
}

/* Check tracked keys and push repeat events when due. */
void SDLApp::UpdateKeyRepeats()
{
#if MUZI_HAVE_SDL
	uint32_t now = SDL_GetTicks();
	for (int i = 0; i < MAX_TRACKED_KEYS; ++i) {
		if (!key_states_[i].used) continue;
		if (!key_states_[i].down) continue;
		if ((int32_t)(key_states_[i].next_repeat_ms - now) <= 0) {
			/* time to push a repeat event */
			ui_event_t kev = {0};
			kev.type = UI_EVENT_KEY;
			int32_t kc = key_states_[i].keycode;
			uint16_t mapped = 0;
			switch (kc) {
				case SDLK_LEFT: mapped = KEY_ARROW_LEFT; break;
				case SDLK_RIGHT: mapped = KEY_ARROW_RIGHT; break;
				case SDLK_UP: mapped = KEY_ARROW_UP; break;
				case SDLK_DOWN: mapped = KEY_ARROW_DOWN; break;
				case SDLK_BACKSPACE: mapped = KEY_BACKSPACE; break;
				case SDLK_RETURN: mapped = KEY_ENTER; break;
				case SDLK_TAB: mapped = KEY_TAB; break;
				case SDLK_ESCAPE: mapped = KEY_ESCAPE; break;
				case SDLK_SPACE: mapped = KEY_SPACE; break;
				default:
					if (kc >= 32 && kc < 128) mapped = (uint16_t)kc;
					break;
			}
			kev.payload.keyboard.keyCode = mapped;
			event_push(&kev);
			key_states_[i].next_repeat_ms = now + KEY_REPEAT_INTERVAL_MS;
		}
	}
#endif
}

SDLApp::~SDLApp() {
#if MUZI_HAVE_SDL
	if (sdl_renderer_) {
		SDL_DestroyRenderer((SDL_Renderer*)sdl_renderer_);
		sdl_renderer_ = NULL;
	}
	if (sdl_window_) {
		SDL_DestroyWindow((SDL_Window*)sdl_window_);
		sdl_window_ = NULL;
	}
    if (sdl_texture_) {
        SDL_DestroyTexture((SDL_Texture*)sdl_texture_);
        sdl_texture_ = NULL;
    }
	if (sdl_initialized_) {
		SDL_Quit();
		sdl_initialized_ = false;
	}
#endif
}

/* Ensure an SDL texture of the requested size exists (create or recreate). */
bool SDLApp::EnsureTexture(int tex_w, int tex_h)
{
#if MUZI_HAVE_SDL
	SDL_Renderer *renderer = (SDL_Renderer*)sdl_renderer_;
	if (!sdl_texture_) {
		SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, tex_w, tex_h);
		if (!tex) {
			fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
			APP_LOG("SDL_CreateTexture failed: %s", SDL_GetError());
			return false;
		}
		sdl_texture_ = (void*)tex;
		return true;
	}
	SDL_Texture *tex = (SDL_Texture*)sdl_texture_;
	int w = 0, h = 0;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	if (w != tex_w || h != tex_h) {
		SDL_DestroyTexture(tex);
		SDL_Texture *newtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, tex_w, tex_h);
		if (!newtex) {
			fprintf(stderr, "SDL_CreateTexture failed (resize): %s\n", SDL_GetError());
			APP_LOG("SDL_CreateTexture failed (resize): %s", SDL_GetError());
			sdl_texture_ = NULL;
			return false;
		}
		sdl_texture_ = (void*)newtex;
	}
	return true;
#else
	return false;
#endif
}


/* Render the headless text buffer into the SDL texture. */
void SDLApp::RenderBufferToTexture(const uint8_t *buffer, int cols, int rows, int glyph_w, int glyph_h)
{
#if MUZI_HAVE_SDL
	if (!sdl_texture_) return;
	SDL_Texture *tex = (SDL_Texture*)sdl_texture_;
	void *pixels = NULL;
	int pitch = 0;
	if (SDL_LockTexture(tex, NULL, &pixels, &pitch) != 0) {
		fprintf(stderr, "SDL_LockTexture failed: %s\n", SDL_GetError());
		APP_LOG("SDL_LockTexture failed: %s", SDL_GetError());
		return;
	}

	/* Simple 16-color palette (ARGB8888) */
	const uint32_t palette[16] = {
		0xFF000000, /* 0 black */
		0xFF0000AA, /* 1 blue */
		0xFF00AA00, /* 2 green */
		0xFF00AAAA, /* 3 cyan */
		0xFFAA0000, /* 4 red */
		0xFFAA00AA, /* 5 magenta */
		0xFFAA5500, /* 6 brown */
		0xFFAAAAAA, /* 7 light gray */
		0xFF555555, /* 8 dark gray */
		0xFF5555FF, /* 9 light blue */
		0xFF55FF55, /* 10 light green */
		0xFF55FFFF, /* 11 light cyan */
		0xFFFF5555, /* 12 light red */
		0xFFFF55FF, /* 13 light magenta */
		0xFFFFFF55, /* 14 yellow */
		0xFFFFFFFF  /* 15 white */
	};

	uint8_t *dst_row = (uint8_t*)pixels;
	const uint8_t *src = buffer;
	const int glyph_h_local = glyph_h; /* keep name tidy for loop capture */

	for (int row = 0; row < rows; ++row) {
		for (int glyph_row = 0; glyph_row < glyph_h_local; ++glyph_row) {
			uint32_t *dst_pixel = (uint32_t*)(dst_row + glyph_row * pitch + 0);
			for (int col = 0; col < cols; ++col) {
				uint8_t ch = src[2 * col + 0];
				uint8_t color = src[2 * col + 1];
				uint8_t fg = color & 0x0F;
				uint8_t bg = (color >> 4) & 0x0F;
				const uint8_t *glyph = &font_data[(size_t)ch * glyph_h_local + glyph_row];
				uint8_t bits = *glyph;
				for (int bit = 0; bit < glyph_w; ++bit) {
					bool set = (bits & (1 << (7 - bit))) != 0;
					uint32_t px = set ? palette[fg] : palette[bg];
					dst_pixel[col * glyph_w + bit] = px;
				}
			}
		}
		src += cols * 2;
		dst_row += glyph_h_local * pitch;
	}

	SDL_UnlockTexture(tex);
#endif
}


/* Poll SDL events and map them to ui_event_t, pushing via event_push().
 * Coordinates are mapped using the provided destination rectangle and
 * integer scale so clicks map to text cell coordinates.
 */
void SDLApp::ProcessSDLEvents(int cols, int rows, int glyph_w, int glyph_h, int dst_x, int dst_y, int scale, bool &running)
{
#if MUZI_HAVE_SDL
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		if (ev.type == SDL_QUIT) {
			running = false;
			break;
		}
		/* Handle key down/up ourselves; we implement delayed repeat logic.
		 * We'll ignore SDL's repeat flag and manage timings in UpdateKeyRepeats. */
		if (ev.type == SDL_KEYDOWN) {
			SDL_Keycode kc = ev.key.keysym.sym;
			/* Find existing or free slot for this key */
			int free_idx = -1;
			int found_idx = -1;
			for (int i = 0; i < MAX_TRACKED_KEYS; ++i) {
				if (key_states_[i].used) {
					if (key_states_[i].keycode == (int32_t)kc) { found_idx = i; break; }
				} else if (free_idx == -1) {
					free_idx = i;
				}
			}

			uint32_t now = SDL_GetTicks();
			if (found_idx == -1) {
				if (free_idx != -1) {
					key_states_[free_idx].used = true;
					key_states_[free_idx].down = true;
					key_states_[free_idx].keycode = (int32_t)kc;
					key_states_[free_idx].next_repeat_ms = now + KEY_REPEAT_DELAY_MS;
				}
				/* push initial key event immediately */
				ui_event_t kev = {0};
				kev.type = UI_EVENT_KEY;
				uint16_t mapped = 0;
				switch (kc) {
					case SDLK_LEFT: mapped = KEY_ARROW_LEFT; break;
					case SDLK_RIGHT: mapped = KEY_ARROW_RIGHT; break;
					case SDLK_UP: mapped = KEY_ARROW_UP; break;
					case SDLK_DOWN: mapped = KEY_ARROW_DOWN; break;
					case SDLK_BACKSPACE: mapped = KEY_BACKSPACE; break;
					case SDLK_RETURN: mapped = KEY_ENTER; break;
					case SDLK_TAB: mapped = KEY_TAB; break;
					case SDLK_ESCAPE: mapped = KEY_ESCAPE; break;
					case SDLK_SPACE: mapped = KEY_SPACE; break;
					default:
						if (kc >= 32 && kc < 128) mapped = (uint16_t)kc;
						break;
				}
				kev.payload.keyboard.keyCode = mapped;
				event_push(&kev);
				if (kc == SDLK_ESCAPE) { running = false; break; }
			} else {
				/* already tracked as down: ignore SDL repeat events; we'll generate repeats */
			}
			continue;
		}

		if (ev.type == SDL_KEYUP) {
			SDL_Keycode kc = ev.key.keysym.sym;
			for (int i = 0; i < MAX_TRACKED_KEYS; ++i) {
				if (key_states_[i].used && key_states_[i].keycode == (int32_t)kc) {
					key_states_[i].used = false;
					key_states_[i].down = false;
					key_states_[i].keycode = 0;
					key_states_[i].next_repeat_ms = 0;
					break;
				}
			}
			continue;
		}

		if (ev.type == SDL_MOUSEMOTION || ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP) {
			ui_event_t mev = {0};
			if (ev.type == SDL_MOUSEMOTION) mev.type = UI_EVENT_MOUSEMOVE; else if (ev.type == SDL_MOUSEBUTTONDOWN) mev.type = UI_EVENT_MOUSEDOWN; else mev.type = UI_EVENT_MOUSEUP;

			int px = (ev.type == SDL_MOUSEMOTION) ? ev.motion.x : ev.button.x;
			int py = (ev.type == SDL_MOUSEMOTION) ? ev.motion.y : ev.button.y;
			int tx = (px - dst_x) / scale;
			int ty = (py - dst_y) / scale;
			if (tx < 0) tx = 0; if (ty < 0) ty = 0;
			int col = tx / glyph_w;
			int row = ty / glyph_h;
			if (col < 0) col = 0; if (col >= cols) col = cols - 1;
			if (row < 0) row = 0; if (row >= rows) row = rows - 1;

			mev.payload.mouse.x = (uint16_t)col;
			mev.payload.mouse.y = (uint16_t)row;
			mev.payload.mouse.deltaX = (ev.type == SDL_MOUSEMOTION) ? ev.motion.xrel : 0;
			mev.payload.mouse.deltaY = (ev.type == SDL_MOUSEMOTION) ? ev.motion.yrel : 0;
			mev.payload.mouse.buttons = 0;
			if (ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP) {
				if (ev.button.button == SDL_BUTTON_LEFT) mev.type = (ev.type == SDL_MOUSEBUTTONDOWN) ? UI_EVENT_MOUSEDOWN : UI_EVENT_MOUSEUP;
				else if (ev.button.button == SDL_BUTTON_RIGHT) mev.type = (ev.type == SDL_MOUSEBUTTONDOWN) ? UI_EVENT_MOUSEDOWN_R : UI_EVENT_MOUSEUP_R;
				mev.payload.mouse.buttons = ev.button.button;
			}

			event_push(&mev);
			continue;
		}
	}
#endif
}

int SDLApp::Setup() {
	APP_LOG("SDLApp::Setup");
#if MUZI_HAVE_SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		/* Always print SDL errors to stderr so we can diagnose at runtime. */
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		APP_LOG("SDL_Init failed: %s", SDL_GetError());
		return -1;
	}
	sdl_initialized_ = true;

	SDL_Window* win = SDL_CreateWindow("MuziMake",
									   SDL_WINDOWPOS_CENTERED,
									   SDL_WINDOWPOS_CENTERED,
									   800, 600,
									   SDL_WINDOW_SHOWN);
	if (!win) {
		fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
		APP_LOG("SDL_CreateWindow failed: %s", SDL_GetError());
		SDL_Quit();
		sdl_initialized_ = false;
		return -1;
	}
	sdl_window_ = (void*)win;

	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!ren) {
		fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
		APP_LOG("SDL_CreateRenderer failed: %s", SDL_GetError());
		SDL_DestroyWindow(win);
		sdl_window_ = NULL;
		SDL_Quit();
		sdl_initialized_ = false;
		return -1;
	}
	sdl_renderer_ = (void*)ren;
	/* Prefer nearest-neighbor when scaling pixel-art fonts */
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    sdl_texture_ = NULL;
	/* Initialize textmode emulation and event subsystem so MainUI works
	 * without depending on DOS-specific startup. This mirrors DOSApp::Setup. */
	textmode_setmode(3);
	textmode_init_font(font_data, 8, 0, 256);
	event_init();
	atexit(event_shutdown);

	return 0;
#else
	APP_LOG("SDL not available at compile time; SDLApp setup is a no-op");
	return 0;
#endif
}

int SDLApp::MainLoop() {
	APP_LOG("SDLApp::MainLoop");
#if MUZI_HAVE_SDL
	if (!sdl_initialized_) return 0;

	bool running = true;
	MainUI ui;
	bool init_ok = ui.Initialize();
	(void)init_ok;

	while (running && !ui.IsDone()) {
			/* Determine cols/rows from current mode so we can map mouse coords
			 * while polling SDL events. Use modeinfo to avoid allocating a buffer
			 * prematurely. */
			MODEINFO *mode = textmode_get_modeinfo();
			const int cols = mode->numCols;
			const int rows = mode->numRows;
			const int glyph_w = 8;
			const int glyph_h = 8;
			const int tex_w = cols * glyph_w;
			const int tex_h = rows * glyph_h;

			SDL_Renderer *renderer = (SDL_Renderer*)sdl_renderer_;

			if (cols == 0 || rows == 0) {
				/* nothing to draw; clear and present */
				SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x30, 0xff);
				SDL_RenderClear(renderer);
				SDL_RenderPresent(renderer);
				SDL_Delay(10);
				continue;
			}

			/* calculate integer scale and destination rectangle (used for
			 * mapping mouse coords). We compute this now from modeinfo and
			 * window size. */
			int win_w = 0, win_h = 0;
			SDL_GetWindowSize((SDL_Window*)sdl_window_, &win_w, &win_h);
			int scale_w = win_w / tex_w;
			int scale_h = win_h / tex_h;
			int scale = scale_w < scale_h ? scale_w : scale_h;
			if (scale < 1) scale = 1;
			int dst_w = tex_w * scale;
			int dst_h = tex_h * scale;
			int dst_x = (win_w - dst_w) / 2;
			int dst_y = (win_h - dst_h) / 2;

			/* Poll SDL events (push -> queued events) and let MainUI consume them */
			ProcessSDLEvents(cols, rows, glyph_w, glyph_h, dst_x, dst_y, scale, running);

			/* Now let MainUI consume queued events */
			/* Also update repeat timers and push repeat events if needed */
			UpdateKeyRepeats();
			ui.HandleEvents();

			/* Render the text-mode UI into the headless buffer now that events
			 * have been processed, then blit and present. */
			ui.RenderAll(true);
			textbuffer_t screen = textmode_get_screen();
			if (screen.buffer == NULL) {
				/* nothing to draw; clear and present */
				SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x30, 0xff);
				SDL_RenderClear(renderer);
				SDL_RenderPresent(renderer);
				textmode_dispose_buffer(&screen);
				SDL_Delay(10);
				continue;
			}

			if (!EnsureTexture(tex_w, tex_h)) {
				textmode_dispose_buffer(&screen);
				break;
			}

			RenderBufferToTexture(screen.buffer, cols, rows, glyph_w, glyph_h);

			/* present */
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
			SDL_RenderClear(renderer);
			SDL_Rect dstrect = { dst_x, dst_y, dst_w, dst_h };
			SDL_RenderCopy(renderer, (SDL_Texture*)sdl_texture_, NULL, &dstrect);
			SDL_RenderPresent(renderer);

			textmode_dispose_buffer(&screen);

			/* Give up some CPU if nothing happening */
			SDL_Delay(10);
	}

	ui.Shutdown();

	return 0;
#else
	APP_LOG("SDL not present; MainLoop exits immediately");
	return 0;
#endif
}
