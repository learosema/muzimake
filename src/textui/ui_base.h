
#ifndef __UI_BASE_H__
#define __UI_BASE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "geometry.h"
#include <stdint.h>

typedef bool (*event_handler_t)(uint16_t, void *);

typedef struct ui_generic_s
{
	uint16_t id;
	rect_t bounding_rect;
	uint8_t color;
	bool active;
	bool focused;
	bool paint;
	event_handler_t *event_handler;
} ui_generic_t;

#ifdef __cplusplus
}
#endif
#endif
