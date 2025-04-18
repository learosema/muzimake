#include "textmode.h"
#include "ui_piano.h"
#include "cp437.h"

void piano_render(ui_piano_t *piano)
{
	const uint8_t keys[7] = {1, 1, 0, 1, 1, 1, 0};
	uint8_t color = piano->color;

	if (piano->bounding_rect.height < 7) {
		piano->bounding_rect.height = 7;
	}
	if (piano->bounding_rect.width < 14) {
		piano->bounding_rect.width = 14;
	}
	rect_t inner = get_clientrect(&(piano->bounding_rect));
	uint8_t black_height = inner.height * 0.7;

	if (piano->focused) {
		textmode_putchar_color(
			piano->bounding_rect.x,
			piano->bounding_rect.y,
			CP_THIN_RIGHT_THICK_DOWN,
			piano->color
		);
		textmode_putchar_color(
			piano->bounding_rect.x + piano->bounding_rect.width - 1,
			piano->bounding_rect.y,
			CP_THIN_LEFT_THICK_DOWN,
			piano->color
		);
		textmode_putchar_color(
			piano->bounding_rect.x,
			piano->bounding_rect.y + piano->bounding_rect.height -1,
			CP_THIN_RIGHT_THICK_UP,
			piano->color
		);
		textmode_putchar_color(
			piano->bounding_rect.x + piano->bounding_rect.width - 1,
			piano->bounding_rect.y + piano->bounding_rect.height -1,
			CP_THIN_LEFT_THICK_UP,
			piano->color
		);

		textmode_vline(piano->bounding_rect.x,
			inner.y, inner.height, CP_THICK_VERTICAL, piano->color);
		textmode_vline(piano->bounding_rect.x + + piano->bounding_rect.width - 1,
			inner.y, inner.height, CP_THICK_VERTICAL, piano->color);

		for (uint8_t innerX = 0; innerX < inner.width; innerX++) {
			char chTop = (innerX % 2 == 0) ?
				CP_THIN_HORIZONTAL :
				(keys[((innerX>>1) % 7)]) ? CP_LOWER_BLOCK :
				CP_THIN_HORIZONTAL_THIN_DOWN;
			char chBottom = (innerX % 2 == 0) ?
				CP_THIN_HORIZONTAL :
				CP_THIN_HORIZONTAL_THIN_UP;

			textmode_putchar_color(
				inner.x + innerX,
				piano->bounding_rect.y, chTop, color
			);
			textmode_putchar_color(
				inner.x + innerX,
				piano->bounding_rect.y + piano->bounding_rect.height - 1,
				chBottom, color
			);
		}
	} else {
		textmode_putchar_color(
			piano->bounding_rect.x,
			piano->bounding_rect.y,
			CP_THIN_RIGHT_THIN_DOWN,
			piano->color
		);
		textmode_putchar_color(
			piano->bounding_rect.x + piano->bounding_rect.width - 1,
			piano->bounding_rect.y,
			CP_THIN_LEFT_THIN_DOWN,
			piano->color
		);
		textmode_putchar_color(
			piano->bounding_rect.x,
			piano->bounding_rect.y + piano->bounding_rect.height -1,
			CP_THIN_RIGHT_THIN_UP,
			piano->color
		);
		textmode_putchar_color(
			piano->bounding_rect.x + piano->bounding_rect.width - 1,
			piano->bounding_rect.y + piano->bounding_rect.height -1,
			CP_THIN_LEFT_THIN_UP,
			piano->color
		);

		textmode_vline(piano->bounding_rect.x,
			inner.y, inner.height, CP_THIN_VERTICAL, piano->color);
		textmode_vline(piano->bounding_rect.x + + piano->bounding_rect.width - 1,
			inner.y, inner.height, CP_THIN_VERTICAL, piano->color);

		for (uint8_t innerX = 0; innerX < inner.width; innerX++) {
			char chTop = (innerX % 2 == 0) ?
				CP_THIN_HORIZONTAL :
				(keys[((innerX>>1) % 7)]) ? CP_LOWER_BLOCK :
				CP_THIN_HORIZONTAL_THIN_DOWN;
			char chBottom = (innerX % 2 == 0) ?
				CP_THIN_HORIZONTAL :
				CP_THIN_HORIZONTAL_THIN_UP;

			textmode_putchar_color(
				inner.x + innerX,
				piano->bounding_rect.y, chTop, color
			);
			textmode_putchar_color(
				inner.x + innerX,
				piano->bounding_rect.y + piano->bounding_rect.height - 1,
				chBottom, color
			);
		}
	}

	for (uint8_t y = 0; y < inner.height; y++) {
		for (uint8_t x = 0; x < inner.width; x++) {
			char ch = (x%2 == 0) ? ' ' :
				(y < black_height * keys[((x>>1) % 7)]) ? CP_BLOCK : CP_THIN_VERTICAL;
			textmode_putchar_color(inner.x + x, inner.y + y, ch, color);
		}
	}
}

void piano_process_events(ui_piano_t *piano, ui_event_t *event)
{
	if (event->type == UI_EVENT_MOUSEUP) {
		ui_handle_mouseup((ui_generic_t *)piano, event);
 	}
	// todo: keyboard/mouse handling
}
