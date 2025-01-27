#ifndef __MACROS_H__
#define __MACROS_H__

#define ALLOC_TYPE(T, N) ((T *)malloc(sizeof(T) * N))
#define REALLOC_TYPE(P, T, N) ((T *)realloc(P, sizeof(T) * N))




#endif
