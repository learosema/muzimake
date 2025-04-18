#include "textmode.h"
#include "macros.h"
#include "ui_sheet.h"

void sheet_render(ui_sheet_t *sheet)
{
	const uint8_t keys[7] = {1, 1, 0, 1, 1, 1, 0};
	uint8_t color = sheet->color;

	if (sheet->bounding_rect.height < 7) {
		sheet->bounding_rect.height = 7;
	}
	if (sheet->bounding_rect.width < 14) {
		sheet->bounding_rect.width = 14;
	}
	rect_t inner = get_clientrect(&(sheet->bounding_rect));
	uint8_t black_height = inner.height * 0.7;

	sheet->bounding_rect.width = MAX(sheet->bounding_rect.width, 2 + 9 * 8);

	if (sheet->focused) {
		textmode_dblrect(
			sheet->bounding_rect.x,
			sheet->bounding_rect.y,
			sheet->bounding_rect.width,
			sheet->bounding_rect.height,
			color
		);
	} else {
		textmode_rect(
			sheet->bounding_rect.x,
			sheet->bounding_rect.y,
			sheet->bounding_rect.width,
			sheet->bounding_rect.height,
			color
		);
	}

	textmode_colorize_area(inner.x, inner.y, inner.width, inner.height, sheet->color);
	uint8_t num_voices = 9;
	for (uint8_t row = 0; row < inner.height; row++) {
		for (uint8_t voice = 0; voice < num_voices; voice++) {
			 // todo print actual data here...
			textmode_print(".......", inner.x + voice * 8, inner.y + row, color);
			if (voice < num_voices) {
				textmode_putchar_color(inner.x + voice * 8 + 7, inner.y + row, ' ', color);
			}
		}
	}
}

void sheet_process_events(ui_sheet_t *sheet, ui_event_t *event)
{
	if (event->type == UI_EVENT_MOUSEUP) {
		ui_handle_mouseup((ui_generic_t *)sheet, event);
 	}
	// todo: keyboard/mouse handling
}
