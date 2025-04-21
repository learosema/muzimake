#include "textmode.h"
#include "ui_lsbox.h"
#include "cp437.h"
#include "macros.h"

void listbox_render(ui_listbox_t *listbox)
{
	uint8_t lbcolor = listbox->color;
	const uint8_t symbol_topleft = listbox->focused ?
		CP_THICK_RIGHT_THICK_DOWN : CP_THIN_RIGHT_THIN_DOWN;
	const uint8_t symbol_horizontal = listbox->focused ?
		CP_THICK_HORIZONTAL : CP_THIN_HORIZONTAL;
	const uint8_t symbol_topright = listbox->focused ?
		CP_THICK_LEFT_THICK_DOWN : CP_THIN_LEFT_THIN_DOWN;
	const uint8_t symbol_vertical = listbox->focused ?
		CP_THICK_VERTICAL : CP_THIN_VERTICAL;
	const uint8_t symbol_bottomright = listbox->focused ?
		CP_THICK_LEFT_THICK_UP : CP_THIN_LEFT_THIN_UP;
	const uint8_t symbol_bottomleft= listbox->focused ?
		CP_THICK_RIGHT_THICK_UP : CP_THIN_RIGHT_THIN_UP;

	rect_t clientrect = get_clientrect(&(listbox->bounding_rect));

	uint8_t x0 = listbox->bounding_rect.x;
	uint8_t y0 = listbox->bounding_rect.y;
	uint8_t x1 = x0 + listbox->bounding_rect.width - 1;
	uint8_t y1 = y0 + listbox->bounding_rect.height - 1;
	uint8_t w = listbox->bounding_rect.width;
	uint8_t h = listbox->bounding_rect.height;

	textmode_putchar_color(x0, y0, symbol_topleft, lbcolor);
	textmode_putchar_color(x1, y0, symbol_topright, lbcolor);
	textmode_putchar_color(x0, y1, symbol_bottomleft, lbcolor);
	textmode_putchar_color(x1, y1, symbol_bottomright, lbcolor);

	if (w > 2) {
		textmode_hline(x0 + 1, y0, w - 2, symbol_horizontal, lbcolor);
		textmode_hline(x0 + 1, y1, w - 2, symbol_horizontal, lbcolor);
	}
	if (h > 2) {
		textmode_vline(x0, y0 + 1, h - 2, symbol_vertical, lbcolor);
		if (listbox->num_items == 0 || clientrect.height >= listbox->num_items) {
			// do not show a scroll line when the list is empty or the listbox is
			// big enough
			textmode_vline(x1, y0 + 1, h - 2, symbol_vertical, lbcolor);
		} else {
			textmode_putchar_color(x1, y0 + 1, CP_UP_TRIANGLE, lbcolor);
			textmode_putchar_color(x1, y0 + h - 2, CP_DOWN_TRIANGLE, lbcolor);
			uint8_t scrollbarheight = clientrect.height - 2; // the bar height without arrows
			float thumb_f = (float)(clientrect.height) / (float)(listbox->num_items);
			uint8_t thumb_h = CLAMP(
				(0.5f + (float)(scrollbarheight) * thumb_f), 1, scrollbarheight);
			uint8_t thumb_y0 = 2 + CLAMP((uint8_t)(
				(float)(listbox->cursor_y0) / (float)(listbox->num_items) * (float)(scrollbarheight) + 0.5
			), 0, (h-4));

			for (uint8_t scroll_y = 2; scroll_y < h - 2; scroll_y++) {
				textmode_putchar_color(x1, y0 + scroll_y,
					((scroll_y >= thumb_y0) &&
					(scroll_y <(thumb_y0 + thumb_h))) ? CP_BLOCK : CP_MEDIUM_SHADE, lbcolor);
			}
		}
	}

	uint8_t inner_width = listbox->bounding_rect.width - 2;
	uint8_t inner_height = listbox->bounding_rect.height - 2;
	uint8_t select_color = 0x70;
	for (uint8_t y = 0; y < inner_height; y++)
	{
		uint16_t yy = listbox->cursor_y0 + y;
		uint8_t color = yy == (listbox->cursor_y0 + listbox->cursor_y) ?
			select_color : lbcolor;
		if (yy < listbox->num_items) {
			uint8_t written = textmode_printn_color(listbox->values[yy], inner_width,
				listbox->bounding_rect.x + 1,
				listbox->bounding_rect.y + 1 + y, color
			);
			textmode_hline(
				listbox->bounding_rect.x + 1 + written,
				listbox->bounding_rect.y + 1 + y,
				inner_width - written, ' ', color
			);
		} else {
			textmode_hline(
				listbox->bounding_rect.x + 1,
				listbox->bounding_rect.y + 1 + y,
				inner_width, ' ', color
			);
		}
	}
}

void listbox_process_events(ui_listbox_t *listbox, ui_event_t *event)
{
	switch (event->type) {
		case UI_EVENT_MOUSEUP: {
			ui_handle_mouseup((ui_generic_t *)listbox, event);
			rect_t clientrect = get_clientrect(&(listbox->bounding_rect));
			if (rect_test_mouse(&clientrect, event->payload.mouse.x, event->payload.mouse.y)) {
				uint8_t y = (uint8_t)(event->payload.mouse.y / 8) - clientrect.y;
				if (y + listbox->cursor_y0 < listbox->num_items) {
					listbox->cursor_y = y;
					listbox->paint = true;
				}
			}
			break;
		}
		case UI_EVENT_KEY: {
			if (! listbox->focused) {
				return;
			}
			uint16_t inner_height = listbox->bounding_rect.height - 2;
			if (event->payload.keyboard.keyCode == KEY_ARROW_DOWN) {
				if (listbox->cursor_y0 + listbox->cursor_y < listbox->num_items - 1) {
					if (listbox->cursor_y < inner_height - 1) {
						listbox->cursor_y += 1;
					} else {
						listbox->cursor_y0 += 1;
					}
					listbox->paint = true;
				}
			}
			if (event->payload.keyboard.keyCode == KEY_ARROW_UP) {
				if (listbox->cursor_y > 0) {
					listbox->cursor_y -= 1;
					listbox->paint = true;
				} else {
					if (listbox->cursor_y0 > 0) {
						listbox->cursor_y0 -= 1;
						listbox->paint = true;
					}
				}
			}
		}
	}
}
