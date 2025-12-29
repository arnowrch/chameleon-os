/**
 * @file lvgl_ui.h
 * @brief Main UI Initialization and Screen Prototypes
 */

#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the entire UI system (theme, router, first screen).
 */
void lvgl_ui_init(void);

// ============================================================================
// Screen Prototypes
// ============================================================================

/**
 * @brief Creates the Home Dashboard Screen.
 * 
 * @return lv_obj_t* The created screen object.
 */
lv_obj_t* create_screen_home(void);

/**
 * @brief Creates the Settings Screen.
 * 
 * @return lv_obj_t* The created screen object.
 */
lv_obj_t* create_screen_settings(void);

/**
 * @brief Creates the Room List Screen.
 * 
 * @return lv_obj_t* The created screen object.
 */
lv_obj_t* create_screen_room_list(void);

/**
 * @brief Creates the Heart Luxury Screen (Example of a complex widget).
 * 
 * @return lv_obj_t* The created screen object.
 */
lv_obj_t* create_screen_heart_luxury(void);


#ifdef __cplusplus
}
#endif

#endif // LVGL_UI_H
