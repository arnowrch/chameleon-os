/**
 * @file ui_router.h
 * @brief Screen Router for Navigation and State Management
 * 
 * Defines the structure for managing different screens (views) in the application.
 */

#ifndef UI_ROUTER_H
#define UI_ROUTER_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Screen IDs
// ============================================================================

typedef enum {
    SCREEN_ID_HOME = 0,
    SCREEN_ID_SETTINGS,
    SCREEN_ID_ROOM_LIST,
    SCREEN_ID_HEART_LUXURY,
    SCREEN_ID_COUNT, // Total number of screens
} screen_id_t;

// ============================================================================
// Public Functions
// ============================================================================

/**
 * @brief Initialize all screens and the router.
 * 
 * Should be called once after LVGL initialization.
 */
void ui_router_init(void);

/**
 * @brief Navigate to a specific screen ID.
 * 
 * Handles screen creation (if not exists), transition, and cleanup of the old screen.
 * 
 * @param screen_id The ID of the target screen.
 */
void ui_router_go(screen_id_t screen_id);

/**
 * @brief Get the currently active screen object.
 * 
 * @return lv_obj_t* The active screen object.
 */
lv_obj_t* ui_router_get_active_screen(void);

#ifdef __cplusplus
}
#endif

#endif // UI_ROUTER_H
