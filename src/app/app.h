#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the application. This should be called before any other app functions.
 */
int app_init();

/**
 * Shutdown the application.
 */
int app_shutdown();

/**
 * Main loop of the application. T
 * his should be called after app_init() and will run until the application is ready to exit.
 */
int app_main_loop();

#ifdef __cplusplus
}
#endif
#endif
