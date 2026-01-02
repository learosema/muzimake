#ifndef __UI_EVENT_H__
#define __UI_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ui_event.h>

uint8_t event_poll(ui_event_t *events, uint16_t offset, uint16_t max_events);
void event_clear(ui_event_t *event);
void event_init();
void event_shutdown();
void wait_for_user();

/* Push an event into the platform event queue so `event_poll` will return it.
 * Thread-unsafe; intended to be called from the main thread where SDL events
 * are processed. Returns 0 on success, -1 if the internal queue is full.
 */
int event_push(const ui_event_t *ev);

#ifdef __cplusplus
}
#endif
#endif
