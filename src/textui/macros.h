#ifndef __MACROS_H__
#define __MACROS_H__

#define ALLOC_TYPE(T, N) ((T *)malloc(sizeof(T) * N))
#define CALLOC_TYPE(T, N) ((T *)calloc(sizeof(T) * N))
#define REALLOC_TYPE(P, T, N) ((T *)realloc(P, sizeof(T) * N))

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define CLAMP(X,A,B) MIN(MAX(A,X),B)



#endif
