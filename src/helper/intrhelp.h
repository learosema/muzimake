#ifndef __INTRHELP_H__
#define __INTRHELP_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __DOS__
typedef void (__interrupt __far *interrupt_func_t)();
#define INTERRUPT __interrupt __far
#else
typedef void (*interrupt_func_t)();
#define INTERRUPT
#endif

#ifdef __cplusplus
}
#endif
#endif
