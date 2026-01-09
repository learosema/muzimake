#include <string.h>

#include "macros.h"
#include "txtbuffer.h"
#include "ui_btn.h"
#include "ui_event.h"

void button_render(textbuffer_t * const buffer, const ui_button_t * const button)
{
	uint8_t color = button->color;
	if (button->active) {
		color = color & 0x0f;
	}

	if (button->focused) {
		txtbuffer_dblrect(buffer,
			button->bounding_rect.x,
			button->bounding_rect.y,
			button->bounding_rect.width,
			button->bounding_rect.height,
			color
		);
	} else {
		txtbuffer_rect(
			buffer,
			button->bounding_rect.x,
			button->bounding_rect.y,
			button->bounding_rect.width,
			button->bounding_rect.height,
			color
		);
	}
	if (button->bounding_rect.width <= 2 || button->bounding_rect.height <= 2) {
		return;
	}

	int len = MIN(strlen(button->label), button->bounding_rect.width-2);
	uint8_t centerX = (uint8_t)(button->bounding_rect.x + button->bounding_rect.width/2);
	uint8_t centerY = (uint8_t)(button->bounding_rect.y + button->bounding_rect.height/2);
	uint8_t labelX = (uint8_t)(centerX - len/2);
	uint8_t innerX = button->bounding_rect.x + 1;
	uint8_t innerY = button->bounding_rect.y + 1;
	uint8_t innerWidth = button->bounding_rect.width - 2;
	uint8_t innerHeight = button->bounding_rect.height - 2;
	for (uint8_t j = 0; j < innerHeight; j++) {
		uint8_t currentY = innerY + j;
		if (currentY != centerY) {
			txtbuffer_hline_color(buffer, innerX, currentY, innerWidth, ' ', color);
		} else {
			uint8_t leftWidth = labelX - innerX;
			txtbuffer_hline_color(buffer, innerX, centerY, leftWidth, ' ', color);
			txtbuffer_hline_color(buffer,labelX + len, centerY, innerWidth - len - leftWidth, ' ', color);
			txtbuffer_print_color(buffer, button->label, labelX, centerY, color);
		}
	}
}

void button_active(ui_button_t *const button, const bool active)
{
	button->active = active;
	button->paint = true;
}

void button_focus(ui_button_t * const button, const bool focus)
{
	button->focused = focus;
	button->paint = true;
}

void button_behaviour(ui_event_pool_t * const pool, ui_button_t * const button)
{
	if (ui_event_is_mouse_pressed(pool, 1)) {
		if (rect_test_mouse(&(button->bounding_rect), pool->events[pool->size - 1].payload.mouse.x, pool->events[pool->size - 1].payload.mouse.y)) {
			button->active = true;
			button->paint = true;
			button->focused = true;
		}
	}

	if (ui_event_is_mouse_released(pool, 1)) {
		button->active = false;
		button->paint = true;
		if (rect_test_mouse(&(button->bounding_rect), pool->events[pool->size - 1].payload.mouse.x, pool->events[pool->size - 1].payload.mouse.y)) {
			ui_event_t click = {0};
			click.type = UI_EVENT_CLICK;
			click.payload.click.buttons = 1;
			click.payload.click.target = button->id;
			if (button->event_handler != NULL) {
				button->event_handler(button->id, &click);
			}
			return;
		}
	}

	// Keyboard events
	if (! (button->focused)) {
		return;
	}

	if (ui_event_is_key_pressed(pool, KEY_SCANCODE_ENTER) ||
	    ui_event_is_key_pressed(pool, KEY_SCANCODE_SPACE)) {
		ui_event_t click = {0};
		click.type = UI_EVENT_CLICK;
		click.payload.click.buttons = 1;
		click.payload.click.target = button->id;
		if (button->event_handler != NULL) {
			button->event_handler(button->id, &click);
		}
		return;
	}
}
