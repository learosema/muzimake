#ifndef __TIMER_H__
#define __TIMER_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef void (*timer_func_t)(const uint64_t);

void timer_shutdown();
void timer_init(uint32_t calls_per_second, timer_func_t callback);
void timer_end_of_interrupt();
uint64_t timer_get();

uint16_t timer_calc_freq(uint32_t calls_per_second);
uint16_t timer_get_pit_count();
void timer_set_pit_count(uint16_t c);

#ifdef __cplusplus
}
#endif

#endif
