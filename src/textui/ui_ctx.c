#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "ui_ctx.h"

void ui_context_init(ui_context_t * const ctx)
{
	memset(ctx, 0, sizeof(ui_context_t));
}

void ui_context_begin_frame(ui_context_t * const ctx)
{
	ctx->input.mouse.x = 0;
	ctx->input.mouse.y = 0;
	ctx->input.mouse.buttons = 0;
	ctx->input.mouse.deltaX = 0;
	ctx->input.mouse.deltaY = 0;
	ctx->input.keyboard.text_length = 0;
}

void ui_context_end_frame(ui_context_t * const ctx)
{
	(void)ctx;
}

void ui_context_input_key(ui_context_t * const ctx, const int key_enum, const bool pressed)
{
	if (key_enum < 0 || key_enum >= UI_KEY_MAX) {
		return;
	}
	ctx->input.keyboard.keys[key_enum] = pressed;
}

void ui_context_input_mouse(ui_context_t * const ctx, const uint8_t event_type, const int x, const int y, const int deltaX, const int deltaY, const int buttons)
{
	ctx->input.mouse.last_event = event_type;
	ctx->input.mouse.x = (uint16_t)x;
	ctx->input.mouse.y = (uint16_t)y;
	ctx->input.mouse.deltaX = (int16_t)deltaX;
	ctx->input.mouse.deltaY = (int16_t)deltaY;
	ctx->input.mouse.buttons = (uint16_t)buttons;
}

void ui_context_input_click(ui_context_t * const ctx, const uint8_t event_type, const int buttons)
{
	ctx->input.mouse.last_event = event_type;
	ctx->input.mouse.buttons = (uint16_t)buttons;
}

void ui_context_input_char(ui_context_t * const ctx, const char c)
{
	if (ctx->input.keyboard.text_length < UI_TEXT_INPUT_MAX - 1) {
		ctx->input.keyboard.text[ctx->input.keyboard.text_length] = c;
		ctx->input.keyboard.text_length++;
		ctx->input.keyboard.text[ctx->input.keyboard.text_length] = '\0';
	}
}

/**
 * Query helpers
 */

/** Check if mouse is inside the given rectangle */
bool ui_context_is_mouse_inside_rect(ui_context_t * const ctx, const rect_t * const rect)
{
	return rect_test_mouse(rect, ctx->input.mouse.x, ctx->input.mouse.y);
}

/** Check if mouse has moved */
bool ui_context_is_mouse_moved(ui_context_t * const ctx)
{
	return (ctx->input.mouse.deltaX != 0 || ctx->input.mouse.deltaY != 0);
}

/** Check if mouse is pressed */
bool ui_context_is_mouse_pressed(ui_context_t * const ctx, const int button_id)
{
	return (ctx->input.mouse.buttons & (1 << button_id)) != 0;
}

/** Check if mouse is released */
bool ui_context_is_mouse_released(ui_context_t * const ctx, const int button_id)
{
	return (ctx->input.mouse.buttons & (1 << button_id)) == 0;
}

/** Check if key is pressed */
bool ui_context_is_key_pressed(ui_context_t * const ctx, const int key_enum)
{
	if (key_enum < 0 || key_enum >= UI_KEY_MAX) {
		return false;
	}
	return ctx->input.keyboard.keys[key_enum];
}

/** Check if key is released */
bool ui_context_is_key_released(ui_context_t * const ctx, const int key_enum)
{
	if (key_enum < 0 || key_enum >= UI_KEY_MAX) {
		return false;
	}
	return !ctx->input.keyboard.keys[key_enum];
}

bool ui_context_get_input_text(ui_context_t * const ctx, char *buffer, const size_t buffer_size)
{
	if (ctx->input.keyboard.text_length == 0) {
		return false;
	}
	strncpy(buffer, ctx->input.keyboard.text, buffer_size - 1);
	buffer[buffer_size - 1] = '\0';
	return true;
}
