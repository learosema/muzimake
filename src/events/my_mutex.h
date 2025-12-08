#ifndef __MY_MUTEX_H__
#define __MY_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* === Eigene Mutex-Implementierung (allgemein, C11) === */

typedef struct my_mutex {
	// atomic_flag flag;
	bool flag;
} my_mutex_t;

/* Provide inline implementations to avoid duplicate external symbols when
 * this header is included by multiple translation units. The implementations
 * are no-ops for the host build (they're a placeholder for the DOS spinlock
 * implementation used on older toolchains). */

static inline void my_mutex_init(my_mutex_t* mutex) {
    (void)mutex;
}

static inline void my_mutex_lock(my_mutex_t* mutex) {
    (void)mutex;
}

static inline void my_mutex_unlock(my_mutex_t* mutex) {
    (void)mutex;
}

static inline void my_mutex_destroy(my_mutex_t* mutex) {
    (void)mutex;
}

#ifdef __cplusplus
}
#endif
#endif
