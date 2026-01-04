#ifndef __UI_EVENT_H__
#define __UI_EVENT_H__

#include <stdbool.h>
#include "geometry.h"
/* work in progress: event mirroring? */
void ui_event_begin();
void ui_event_click();
void ui_event_mouse_wheel(const int delta);
void ui_event_mouse_press(const int button_id);
void ui_event_mouse_release(const int button_id);
void ui_event_mouse_move(const int x, const int y, const int deltaX, const int deltaY);
void ui_event_key(const int keyCode, const bool pressed);
void ui_event_end();

bool ui_event_is_mouse_inside_rect(const rect_t * const rect);
bool ui_event_is_mouse_moved();
bool ui_event_is_mouse_pressed(const int button_id);
bool ui_event_is_mouse_released(const int button_id);
bool ui_event_

#endif
