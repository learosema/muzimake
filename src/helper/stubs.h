#ifndef __STUBS_H__
#define __STUBS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define _loadds
#define __interrupt
#define far
#define near

inline void delay(int) { /* stub */ }
inline bool kbhit() { return true; }
inline int getch() { return 0; }
inline int inp(unsigned short) { return 0; }
inline void outp(int, int) { /* stub */ };

#ifdef __cplusplus
}
#endif
#endif

