#ifndef __UI_LOAD_H__
#define __UI_LOAD_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "ui_common.h"
#include "helper/list.h"

typedef struct ui_load_s {
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	uint8_t dirlist_rows;
	uint8_t dirlist_cols;
	ui_event_handler_t event_handler;
	linked_list_t *current_dir;
	int count_files;
	int offset;
	int selected_index;
	uint8_t input_cursor_x;
	char *selected_file;
	bool done;
} ui_load_t;

typedef struct ui_load_result_s {
	char *filename;
} ui_load_result_t;

void ui_load_render(ui_load_t *load_state);
void ui_load_process_events(ui_load_t *load_state, ui_event_t *event);

bool ui_load_modal(ui_load_result_t *result, bool has_mouse);
void ui_load_free_result(ui_load_result_t *result);

#ifdef __cplusplus
}
#endif
#endif
