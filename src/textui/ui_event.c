#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ui_event.h"
#include "ui_common.h"


void ui_event_pool_init(ui_event_pool_t * const pool, const size_t capacity)
{
	if (!pool) return;
	pool->events = NULL;
	pool->size = 0;
	pool->capacity = 0;
	ui_event_ensure_capacity(pool, capacity);
}

void ui_event_pool_dispose(ui_event_pool_t * const pool)
{
	if (!pool) return;
	if (pool->events) {
		free(pool->events);
		pool->events = NULL;
	}
	pool->size = 0;
	pool->capacity = 0;
}

bool pool_ensure_capacity(ui_event_pool_t * const pool, size_t min)
{
	if (pool->capacity >= min) return true;
	size_t newcap = pool->capacity ? pool->capacity * 2 : 16;
	while (newcap < min) newcap *= 2;
	ui_event_t *tmp = (ui_event_t*)realloc(pool->events, newcap * sizeof(ui_event_t));
	if (!tmp) return false;
	pool->events = tmp;
	pool->capacity = newcap;
	return true;
}

bool pool_add_event(ui_event_pool_t * const pool, const ui_event_t * const ev)
{
	if (!pool || !ev) return false;
	if (!pool_ensure_capacity(pool, pool->size + 1)) return false;
	pool->events[pool->size++] = *ev;
	return true;
}

void ui_event_pool_add_mouse_event(ui_event_pool_t * const pool, const ui_event_t * const event)
{

}

/* Frame management */
void ui_event_begin(ui_event_pool_t * const pool)
{
	if (pool->events == NULL) {
		ui_event_pool_init(pool, MAX_EVENTS_PER_FRAME);
	}
	pool->size = 0; /* reuse capacity */
}

void ui_event_click(ui_event_pool_t * const pool)
{
	ui_event_t ev = {0};
	ev.type = UI_EVENT_CLICK;
	ev.payload.click.target = 0;
	ev.payload.click.buttons = 1;
	pool_add_event(pool, &ev);
}

void ui_event_mouse_wheel(ui_event_pool_t * const pool, const int delta)
{
	ui_event_t ev = {0};
	/* represent wheel as a mouse move with deltaY set */
	ev.type = UI_EVENT_MOUSEMOVE;
	ev.payload.mouse.deltaX = 0;
	ev.payload.mouse.deltaY = (int16_t)delta;
	pool_add_event(pool, &ev);
}

void ui_event_mouse_press(ui_event_pool_t * const pool, const int button_id)
{
	ui_event_t ev = {0};
	if (button_id == 2) ev.type = UI_EVENT_MOUSEDOWN_R; else ev.type = UI_EVENT_MOUSEDOWN;
	ev.payload.mouse.buttons = (uint16_t)button_id;
	pool_add_event(pool, &ev);
}

void ui_event_mouse_release(ui_event_pool_t * const pool, const int button_id)
{
	ui_event_t ev = {0};
	if (button_id == 2) ev.type = UI_EVENT_MOUSEUP_R; else ev.type = UI_EVENT_MOUSEUP;
	ev.payload.mouse.buttons = (uint16_t)button_id;
	pool_add_event(pool, &ev);
}

void ui_event_mouse_move(ui_event_pool_t * const pool, const int x, const int y, const int deltaX, const int deltaY)
{
	ui_event_t ev = {0};
	ev.type = UI_EVENT_MOUSEMOVE;
	ev.payload.mouse.x = (uint16_t)x;
	ev.payload.mouse.y = (uint16_t)y;
	ev.payload.mouse.deltaX = (int16_t)deltaX;
	ev.payload.mouse.deltaY = (int16_t)deltaY;
	pool_add_event(pool, &ev);
}

void ui_event_key(ui_event_pool_t * const pool, const int keyCode, const bool pressed)
{
	ui_event_t ev = {0};
	if (pressed) {
		ev.type = UI_EVENT_KEY;
		ev.payload.keyboard.keyCode = (uint16_t)keyCode;
	} else {
		ev.type = UI_EVENT_KEYUP;
		ev.payload.keyboard.keyCode = (uint16_t)keyCode;
	}
	pool_add_event(pool, &ev);
}

void ui_event_end(ui_event_pool_t * const pool)
{
	/* Nothing to commit for now; pool retains frame events until next
	 * ui_event_begin() which will reset the pool->size. */
}

/* Query helpers scan the pooled events (most recent first). */

bool ui_event_is_mouse_inside_rect(ui_event_pool_t * const pool, const rect_t * const rect)
{
	if (!rect || pool->size == 0) return false;
	for (ssize_t i = (ssize_t)pool->size - 1; i >= 0; --i) {
		ui_event_t *ev = &(pool->events[i]);
		if ((ev->type & UI_EVENT_MOUSE) == 0) continue;
		if (rect_test_mouse((rect_t *)rect, ev->payload.mouse.x, ev->payload.mouse.y)) return true;
	}
	return false;
}

bool ui_event_is_mouse_moved(ui_event_pool_t * const pool)
{
	if (pool->size == 0) return false;
	for (ssize_t i = (ssize_t)pool->size - 1; i >= 0; --i) {
		ui_event_t *ev = &(pool->events[i]);
		if (ev->type == UI_EVENT_MOUSEMOVE) {
			if (ev->payload.mouse.deltaX != 0 || ev->payload.mouse.deltaY != 0) return true;
		}
	}
	return false;
}

bool ui_event_is_mouse_pressed(ui_event_pool_t * const pool, const int button_id)
{
	if (pool->size == 0) return false;
	for (ssize_t i = (ssize_t)pool->size - 1; i >= 0; --i) {
		ui_event_t *ev = &(pool->events[i]);
		if ((ev->type & UI_EVENT_MOUSEDOWN) != 0 || ev->type == UI_EVENT_MOUSEDOWN_R) {
			if ((int)ev->payload.mouse.buttons == button_id) return true;
		}
	}
	return false;
}

bool ui_event_is_mouse_released(ui_event_pool_t * const pool, const int button_id)
{
	if (pool->size == 0) return false;
	for (ssize_t i = (ssize_t)pool->size - 1; i >= 0; --i) {
		ui_event_t *ev = &(pool->events[i]);
		if ((ev->type & UI_EVENT_MOUSEUP) != 0 || ev->type == UI_EVENT_MOUSEUP_R) {
			if ((int)ev->payload.mouse.buttons == button_id) return true;
		}
	}
	return false;
}

bool ui_event_is_key_pressed(ui_event_pool_t * const pool, const int keyCode)
{
	if (pool->size == 0) return false;
	for (ssize_t i = (ssize_t)pool->size - 1; i >= 0; --i) {
		ui_event_t *ev = &(pool->events[i]);
		if ((ev->type & UI_EVENT_KEY) != 0) {
			if ((int)ev->payload.keyboard.keyCode == keyCode) return true;
		}
	}
	return false;
}

bool ui_event_is_key_released(ui_event_pool_t * const pool, const int keyCode)
{
	if (pool->size == 0) return false;
	for (ssize_t i = (ssize_t)pool->size - 1; i >= 0; --i) {
		ui_event_t *ev = &(pool->events[i]);
		if ((ev->type & UI_EVENT_KEYUP) != 0) {
			if ((int)ev->payload.keyboard.keyCode == keyCode) return true;
		}
	}
	return false;
}

