#include <stdlib.h>

#include "ui_load.h"
#include "textmode.h"

void display_files(
	ui_load_t *load_state
	/*	linked_list_t *list_files,
	const int offset, const int rows, const int cols, const int colspacing,
	const int x0, const int y0,
	const uint8_t color, const uint8_t selected_color, const int selected_index
*/
)
{
	int n = 0, x = 0, y = 0;

	const int x0 = load_state->bounding_rect.x + 1;
	const int y0 = load_state->bounding_rect.y + 1;
	const int colspacing = 15;
	const int cols = (load_state->bounding_rect.width - 2) / colspacing;
	const int rows = (load_state->bounding_rect.height - 2);
	const int offset = 0; // TODO, too tired right now...
	const int selected_index = 0;
	const uint8_t selected_color = 0x71;
	const uint8_t color = 0x5f;

	for (node_t *iter = load_state->current_dir->head; iter != NULL; iter = iter->next) {
		if (n < offset) {
			n++;
			continue;
		}
		y = (n - offset) / cols;
		x = (n - offset) % cols;

		if (y >= rows) {
			break;
		}

		textmode_print((char *)iter->data, x0 + x * colspacing, y0 + y,
			n == selected_index ? selected_color : color
		);
		n++;
	}
}

rect_t calculate_bounds_cancel(rect_t *bounding_rect)
{
	rect_t result;
	uint8_t x0 = bounding_rect->x;
	uint8_t y0 = bounding_rect->y;
	uint8_t x1 = x0 + bounding_rect->width - 1;
	uint8_t y1 = y0 + bounding_rect->height - 1;
	result.height = 1;
	result.width = 10; // [.Cancel.]
	result.x = x1 - 2 - 10;
	result.y = y1 - 2;
	return result;
}

rect_t calculate_bounds_dirlist(rect_t *bounding_rect)
{
	rect_t result;
	result.x = bounding_rect-> x + 2;
	result.y = bounding_rect-> y + 2;
	result.width = bounding_rect->width - 4;
	result.height = bounding_rect->height - 6;
	return result;
}


void ui_load_render(ui_load_t *load_state)
{
	uint8_t color = 0x5f;
	if (load_state->focused) {
		textmode_dblrect(
			load_state->bounding_rect.x,
			load_state->bounding_rect.y,
			load_state->bounding_rect.width,
			load_state->bounding_rect.height,
			color
		);
	} else {
		textmode_rect(
			load_state->bounding_rect.x,
			load_state->bounding_rect.y,
			load_state->bounding_rect.width,
			load_state->bounding_rect.height,
			color
		);
	}
	textmode_print("Load", load_state->bounding_rect.x + 2, 1, 0x5f);
	rect_t cancel_rect = calculate_bounds_cancel(&(load_state->bounding_rect));
	rect_t dirlist_rect = calculate_bounds_dirlist(&(load_state->bounding_rect));

	textmode_print("[ Cancel ]", cancel_rect.x, cancel_rect.y, 0x5f);
}

void ui_load_process_events(ui_load_t *load_state, ui_event_t *event)
{

}
