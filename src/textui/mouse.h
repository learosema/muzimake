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


#define EVENT_MOUSEMOVE    1
#define EVENT_MOUSEDOWN_L  2
#define EVENT_MOUSEUP_L    4
#define EVENT_MOUSEDOWN_R  8
#define EVENT_MOUSEUP_R    16

#if defined __DOS__ && defined __WATCOM__
#define MOUSE_CALLBACK _loadds far
typedef void (far *far_function_ptr_t)();
#else
#define MOUSE_CALLBACK
typedef void (*far_function_ptr_t)();
#endif

bool mouse_init();
void mouse_show();
void mouse_hide();
void mouse_get_status(MOUSE_STATUS *status);
void mouse_set_vertical_range(uint16_t minY, uint16_t maxY);
void mouse_set_horizontal_range(uint16_t minX, uint16_t maxX);

int mouse_set_eventhandler(far_function_ptr_t handler, uint8_t call_mask);

#ifdef __cplusplus
}
#endif


#endif
