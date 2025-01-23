/**
 * Misc file utils
 */
#ifndef __BYTESWAP_H__
#define __BYTESWAP_H__
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t byteswap16(uint16_t in);
uint32_t byteswap32(uint32_t in);

#ifdef __cplusplus
}
#endif
#endif
