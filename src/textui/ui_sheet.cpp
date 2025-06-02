#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textmode.h"
#include "macros.h"
#include "ui_sheet.h"

static const char* tone_scale[12] = {"C-", "C#", "D-", "D#", "E-", "F-", "F#", "G-", "G#", "A-", "A#", "B-"};

#define NOTE_MAX_VALUE 0x7f
#define NUM_TOTAL_CHANNELS 9
#define CELL_LEN 11
#define MAX_BUF_LEN (NUM_TOTAL_CHANNELS * 11 + 1)

void sheet_build_row(char *row_buf, size_t max_buf_len, pattern_entry_t *data, uint8_t count, uint8_t row_index) {
	row_buf[0] = '\0';
	uint8_t count_clamped = MIN(NUM_TOTAL_CHANNELS, count);
	for (uint8_t channel = 0; channel < count_clamped; channel++) {
		char buf[12] = ".......... ";
		pattern_entry_t entry = data[channel];
		if (entry.has_content && entry.note <= NOTE_MAX_VALUE) {
			uint8_t octave = CLAMP(entry.note / 12, 0, 10);
			const char* tone = tone_scale[entry.note % 12];
			// tone+octave instrument velocity effects(todo)
			snprintf(buf, max_buf_len, "%s%1x%2x%2x... ", tone, octave, entry.instrument, entry.velocity);
		}
		if (entry.has_content && entry.note > NOTE_MAX_VALUE) {
			snprintf(buf, max_buf_len, "---------- ");
		}
		strlcat(row_buf, buf, MAX_BUF_LEN);
	}
}


void sheet_move_cursor(const ui_sheet_t *sheet, const int8_t dx, const int8_t dy)
{
	uint8_t x = sheet->bounding_rect.x + sheet->cursor_x + 5;
	uint8_t y = sheet->bounding_rect.y + sheet->cursor_y + 1;
	mouse_hide();
	textmode_colorize_area(x, y, 1, 1, sheet->color);
	textmode_colorize_area(x + dx, y + dy, 1, 1, INPUT_CURSOR_COLOR);
	mouse_show();
}

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

	int num_cols = sheet->pattern.num_cols;
	int num_rows = sheet->pattern.num_rows;

	char rowBuf[MAX_BUF_LEN+1] = {0};
	for (int y = 0; y < inner.height; y++) {
		int yy = (int)(sheet->offset_y) + y;
		char idx_buf[5] = "\0";
		snprintf(idx_buf, 5, "%02x: ", yy);
		textmode_printn(idx_buf, 4, inner.x, inner.y + y);
		if (yy >= 0 && yy < num_rows) {
			sheet_build_row(rowBuf, MAX_BUF_LEN ,&(sheet->pattern.data[yy * num_cols]), sheet->pattern.num_cols, yy);
			textmode_printn_color(&(rowBuf[sheet->offset_x]), inner.width - 4, inner.x + 4, inner.y + y, sheet->color);
		} else {
			textmode_hline(inner.x + 4, inner.y + y, inner.width - 4, ' ', sheet->color);
		}
	}
	if (sheet->focused) {
		textmode_colorize_area(inner.x + sheet->cursor_x + 4, inner.y + sheet->cursor_y, 1, 1, INPUT_CURSOR_COLOR);
	}
}

void sheet_process_events(ui_sheet_t *sheet, ui_event_t *event)
{
	rect_t inner = get_clientrect(&(sheet->bounding_rect));
	uint8_t absX = sheet->cursor_x + sheet->offset_x;
	uint8_t maxX = sheet->pattern.num_cols * CELL_LEN - 1;
	uint8_t absY = sheet->cursor_y + sheet->offset_y;
	uint8_t maxY = sheet->pattern.num_rows;

	inner.x += 4;
	inner.width -= 4;
	if ((event->type & UI_EVENT_MOUSEUP) > 0) {
		ui_handle_mouseup((ui_generic_t *)sheet, event);
 	}
	if (event->type == UI_EVENT_KEY && sheet->focused) {

		if (event->payload.keyboard.keyCode == KEY_ARROW_RIGHT && absX < maxX - 1) {
			if (sheet->cursor_x < inner.width - 1) {
				sheet_move_cursor(sheet, 1, 0);
				sheet->cursor_x += 1;
			} else {
				sheet->offset_x += 1;
				sheet->paint = true;
			}
			return;
		}
		if (event->payload.keyboard.keyCode == KEY_ARROW_LEFT && absX > 0) {
			if (sheet->cursor_x > 0) {
				sheet_move_cursor(sheet, -1, 0);
				sheet->cursor_x -= 1;
			} else {
				sheet->offset_x -= 1;
				sheet->paint = true;
			}
			return;
		}

		if (event->payload.keyboard.keyCode == KEY_ARROW_DOWN && absY < maxY - 1) {
			if (sheet->cursor_y < inner.height - 1) {
				sheet_move_cursor(sheet, 0, 1);
				sheet->cursor_y += 1;

			} else {
				sheet->offset_y += 1;
				sheet->paint = true;
			}
			return;
		}

		if (event->payload.keyboard.keyCode == KEY_ARROW_UP && absY > 0) {
			if (sheet->cursor_y > 0) {
				sheet_move_cursor(sheet, 0, -1);
				sheet->cursor_y -= 1;
			} else {
				sheet->paint = true;
				sheet->offset_y -= 1;
			}
			return;
		}
	}
}
