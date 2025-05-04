#ifndef __INTRHELP_H__
#define __INTRHELP_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __DOS__
#include <stubs.h>
#endif

typedef void (__interrupt far *interrupt_func_t)();

#ifdef __cplusplus
}
#endif
#endif
