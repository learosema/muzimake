#include <stdlib.h>

#include "ui_load.h"

#include "textui/ui_event.h"
#include "filefmt/fileio.h"
#include "textui/textmode.h"
#include "helper/list.h"

void display_files(
	linked_list_t *list_files,
	rect_t *bounding_rect,
	const int offset,
	const int colspacing,
	const uint8_t color,
	const uint8_t selected_color,
	const int selected_index
)
{
	int n = 0, x = 0, y = 0;

	const int x0 = bounding_rect->x + 1;
	const int y0 = bounding_rect->y + 1;

	const int cols = (bounding_rect->width - 2) / colspacing;
	const int rows = (bounding_rect->height - 2);

	for (node_t *iter = list_files->head; iter != NULL; iter = iter->next) {
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
	result.x = x1 - 2 - 22;
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


void ui_load_render(ui_load_t *state)
{
	uint8_t color = 0x5f;
	if (state->focused) {
		textmode_dblbox(
			state->bounding_rect.x,
			state->bounding_rect.y,
			state->bounding_rect.width,
			state->bounding_rect.height,
			color
		);
	} else {
		textmode_box(
			state->bounding_rect.x,
			state->bounding_rect.y,
			state->bounding_rect.width,
			state->bounding_rect.height,
			color
		);
	}

	textmode_print("Load", state->bounding_rect.x + 2, state->bounding_rect.y, 0x5f);
	rect_t cancel_rect = calculate_bounds_cancel(&(state->bounding_rect));
	rect_t dirlist_rect = calculate_bounds_dirlist(&(state->bounding_rect));
	display_files(state->current_dir, &dirlist_rect, state->offset, 15, 0x5f, 0x71, state->selected_index);
	textmode_print("[ Cancel ]", cancel_rect.x, cancel_rect.y, 0x5f);
	textmode_print("[  Load  ]", cancel_rect.x + 12, cancel_rect.y, 0x5f);
}

void ui_load_change_directory(ui_load_t *state)
{

}

void ui_load_process_events(ui_load_t *state, ui_event_t *event)
{
	int num_cols = (state->bounding_rect.width - 2) / 15;

	if (event->type == UI_EVENT_KEY) {
		// load_onkey(events[i].payload.keyboard.keyCode);
		switch (event->payload.keyboard.keyCode) {
			case KEY_ENTER:
				state->selected_file = (char *)linked_list_node_at(state->current_dir, state->selected_index)->data;
				if (state->selected_file != NULL && state->selected_file[0] == '[') {
					ui_load_change_directory(state);
				} else {
					state->done = true;

				}
				break;
			case KEY_ESCAPE:
				state->done = true;
				state->selected_file = NULL;
				break;
			case KEY_ARROW_LEFT:
				if (state->selected_index > 0) {
					state->selected_index -= 1;
					state->paint = true;
				}
				break;
			case KEY_ARROW_RIGHT:
				if (state->selected_index < state->count_files - 1) {
					state->selected_index += 1;
					state->paint = true;
				}
				break;
			case KEY_ARROW_UP:
				if (state->selected_index >= num_cols ) {
					state->selected_index -= num_cols;
					state->paint = true;
				}
			break;
			case KEY_ARROW_DOWN:
				if (state->selected_index <= state->count_files - 1) {
					state->selected_index += num_cols;
					state->paint = true;
				}
		}
	}
}

bool ui_load_modal(ui_load_result_t *result, bool has_mouse)
{
	if (has_mouse) {
		mouse_hide();
	}
	MODEINFO *info = textmode_get_modeinfo();
	textbuffer_t screen = textmode_get_screen();
	ui_load_t state = {0};

	state.bounding_rect.x = 2;
	state.bounding_rect.y = 2;
	state.bounding_rect.width = info->numCols - 4;
	state.bounding_rect.height = info->numRows - 4;
	state.folder = strdup(".");
	state.current_dir = fileio_list_files(state.folder);
	state.count_files = linked_list_get_count(state.current_dir);

	if (has_mouse) {
		mouse_show();
	}

	ui_event_t events[2] = {0, 0};

	state.paint = true;
	state.done = false;
	while (! state.done) {
		if (state.paint) {
			if (has_mouse) {
				mouse_hide();
			}
			ui_load_render(&state);
			if (has_mouse) {
				mouse_show();
			}
			state.paint = false;
		}
		wait_for_user();
		uint8_t num_events = event_poll(events, 0, 2);
		for (uint8_t i = 0; i < num_events; i++) {
			if ((events[i].type & UI_EVENT_KEY) > 0) {
				textmode_gotoxy(1, 48);
				printf("KEY     %04x\n", events[i].payload.keyboard.keyCode);
			}
			ui_load_process_events(&state, &(events[i]));
		}
	}
	mouse_hide();
	textmode_put_area(&screen, 0, 0);
	if (has_mouse) {
		mouse_show();
	}
	textmode_dispose_buffer(&screen);
	fileio_list_files_dispose(state.current_dir);
	return false;
}

void ui_load_free_result(ui_load_result_t *result)
{
	free(result->filename);
	result->filename = NULL;
}
