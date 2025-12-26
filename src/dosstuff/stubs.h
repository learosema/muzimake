#ifndef __STUBS_H__
#define __STUBS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define _loadds
#define __interrupt
#define far
#define near

inline void delay(int millis) { /* stub */ }
inline bool kbhit() { return true; }
inline int getch() { return 0; }
inline int inp(unsigned short port) { return 0; }
inline void outp(int portno, int value) { /* stub */ };

#ifdef __cplusplus
}
#endif
#endif

