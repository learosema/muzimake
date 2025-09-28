#ifndef __MOUSE_H__
#define __MOUSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <intrhelp.h>
#include <dpmiutil.h>

typedef struct MOUSE_STATUS {
	uint16_t mouseX;
	uint16_t mouseY;
	uint16_t buttons;
} MOUSE_STATUS;

typedef struct mouse_callback_data_s {
	bool has_event;

	uint16_t code;

	uint16_t x_pos;
	uint16_t y_pos;

	int16_t x_counts;
	int16_t y_counts;

	uint16_t button_state;

} mouse_callback_data_t;

#define EVENT_MOUSEMOVE    1
#define EVENT_MOUSEDOWN_L  2
#define EVENT_MOUSEUP_L    4
#define EVENT_MOUSEDOWN_R  8
#define EVENT_MOUSEUP_R    16
#define EVENT_MOUSE_ALL    (EVENT_MOUSEMOVE | EVENT_MOUSEDOWN_L | EVENT_MOUSEUP_L | EVENT_MOUSEDOWN_R | EVENT_MOUSEUP_R)

#if defined __DOS__ && defined __WATCOMC__
typedef void (far *far_function_ptr_t)();
#else
typedef void (*far_function_ptr_t)();
#endif

bool mouse_init();
void mouse_show();
void mouse_hide();
void mouse_get_status(MOUSE_STATUS *status);
void mouse_set_vertical_range(uint16_t minY, uint16_t maxY);
void mouse_set_horizontal_range(uint16_t minX, uint16_t maxX);

int mouse_set_eventhandler(far_function_ptr_t handler, uint8_t call_mask);
int mouse_set_predefined_eventhandler(uint8_t call_mask);
mouse_callback_data_t *mouse_get_callback_data();

#ifdef __cplusplus
}
#endif


#endif
