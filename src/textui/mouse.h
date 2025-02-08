#ifndef __MOUSE_H__
#define __MOUSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct MOUSE_STATUS {
	uint16_t mouseX;
	uint16_t mouseY;
	uint16_t buttons;
} MOUSE_STATUS;


bool mouse_init();
void mouse_show();
void mouse_hide();
void mouse_get_status(MOUSE_STATUS *status);
void mouse_set_vertical_range(uint16_t minY, uint16_t maxY);
void mouse_set_horizontal_range(uint16_t minX, uint16_t maxX);

#ifdef __cplusplus
}
#endif


#endif
