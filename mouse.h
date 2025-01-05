#ifndef __MOUSE_H__
#define __MOUSE_H__

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


#endif