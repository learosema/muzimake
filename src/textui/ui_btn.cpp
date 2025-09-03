#include <string.h>

#include "macros.h"
#include "textmode.h"
#include "ui_btn.h"
#include "keyboard.h"

void button_render(ui_button_t *button)
{
	uint8_t color = button->color;
	if (button->active) {
		color = color & 0x0f;
	}

	if (button->focused) {
		textmode_dblrect(
			button->bounding_rect.x,
			button->bounding_rect.y,
			button->bounding_rect.width,
			button->bounding_rect.height,
			color
		);
	} else {
		textmode_rect(
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
			textmode_hline(innerX, currentY, innerWidth, ' ', color);
		} else {
			uint8_t leftWidth = labelX - innerX;
			textmode_hline(innerX, centerY, leftWidth, ' ', color);
			textmode_hline(labelX + len, centerY, innerWidth - len - leftWidth, ' ', color);
			textmode_print(button->label, labelX, centerY, color);
		}
	}
}

void button_process_events(ui_button_t *button, ui_event_t *event)
{
	if (button->event_handler != nullptr) {
		bool result = button->event_handler(button->id, event);
		if (result == false) {
			return;
		}
	}

	if ((event->type & UI_EVENT_MOUSEMOVE) > 0) {
		if ((button->active)&&
				(rect_test_mouse(&(button->bounding_rect), event->payload.mouse.x, event->payload.mouse.y)))
		{
			button->paint = true;
		}
		return;
	}

	if ((event->type & UI_EVENT_MOUSEDOWN) > 0) {
		if (rect_test_mouse(&(button->bounding_rect), event->payload.mouse.x, event->payload.mouse.y)) {
			button->active = true;
			button->paint = true;
			button->focused = true;
		}
	}

	if ((event->type & UI_EVENT_MOUSEUP) > 0) {
		button->active = false;
		button->paint = true;
		if (rect_test_mouse(&(button->bounding_rect), event->payload.mouse.x, event->payload.mouse.y)) {
			if (button->event_handler != nullptr) {
				ui_event_t click = {0};
				click.type = UI_EVENT_CLICK;
				click.payload.click.buttons = event->payload.mouse.buttons;
				click.payload.click.target = button->id;
				bool result = button->event_handler(button->id, &click);
				if (result == false) {
					return;
				}
			}
		}
	}
/*
	// keyboard events
	kbd_read();
	kbd_state_t * kbd_state = kbd_get_state();

	if (button->focused && button->active == false && (kbd_state->keys[KEY_SCANCODE_ENTER] || kbd_state->keys[KEY_SCANCODE_SPACE])) {
			button->active = true;
			button->paint = true;
	}

	if (button->focused && button->active && !(kbd_state->keys[KEY_SCANCODE_ENTER] || kbd_state->keys[KEY_SCANCODE_SPACE])) {
		button->active = false;
		button->paint = true;
	}
*/
}
