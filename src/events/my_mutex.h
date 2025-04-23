/* === Eigene Mutex-Implementierung (allgemein, C11) === */

typedef struct my_mutex {
	// atomic_flag flag;
	bool flag;
} my_mutex_t;

void my_mutex_init(my_mutex_t* mutex) {
	// atomic_flag_clear(&mutex->flag);
}

void my_mutex_lock(my_mutex_t* mutex) {
	// while (atomic_flag_test_and_set(&mutex->flag)) {
	//     // busy-wait: ggf. kurze Wartezyklen einbauen, um CPU zu schonen.
	// }
}

void my_mutex_unlock(my_mutex_t* mutex) {
	// atomic_flag_clear(&mutex->flag);
}

void my_mutex_destroy(my_mutex_t* mutex) {
	// Kein Cleanup nötig.
}
