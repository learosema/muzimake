#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define COMPONENT_TYPE_BUTTON 1
#define COMPONENT_TYPE_INPUT 2

typedef struct ui_component_s
{
	uint8_t type;


} ui_component_t;


#ifdef __cplusplus
}
#endif
#endif
