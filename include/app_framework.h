/**
 * @file app_framework.h
 * @brief Application framework for Chameleon OS
 * 
 * Manages app lifecycle and switching
 */

#ifndef APP_FRAMEWORK_H
#define APP_FRAMEWORK_H

#include <stdint.h>
#include <stdbool.h>

// App structure
typedef struct App {
    const char* name;
    const char* icon;
    uint8_t sector;  // 1-6
    void (*init)(void);
    void (*on_enter)(void);
    void (*on_update)(void);
    void (*on_exit)(void);
    void (*cleanup)(void);
} App_t;

// Maximum number of apps
#define MAX_APPS 33

/**
 * @brief Initialize app framework
 * @return 0 on success, negative on error
 */
int app_framework_init(void);

/**
 * @brief Register an app
 * @param app Pointer to app structure
 * @return App ID (0-32), or negative on error
 */
int app_register(App_t* app);

/**
 * @brief Get total number of registered apps
 * @return Number of apps
 */
int app_get_count(void);

/**
 * @brief Get app by ID
 * @param app_id App ID (0-32)
 * @return Pointer to app, or NULL if invalid
 */
App_t* app_get(int app_id);

/**
 * @brief Switch to app
 * @param app_id App ID to switch to
 * @return 0 on success, negative on error
 */
int app_switch(int app_id);

/**
 * @brief Get current active app ID
 * @return Current app ID, or -1 if none active
 */
int app_get_current(void);

/**
 * @brief Update current app (call in main loop)
 */
void app_update(void);

#endif // APP_FRAMEWORK_H
