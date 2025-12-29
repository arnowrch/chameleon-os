/**
 * @file ui_router.c
 * @brief Screen Router Implementation
 */

#include "ui_router.h"
#include "ui_theme.h"
#include "ui_components.h"
#include "lvgl_ui.h" // Contains screen creation functions

static lv_obj_t *active_screen = NULL;

/**
 * @brief Function pointer type for screen creation functions.
 */
typedef lv_obj_t *(*screen_create_func_t)(void);

// Array of screen creation functions, mapped by screen_id_t
static const screen_create_func_t screen_create_funcs[SCREEN_ID_COUNT] = {
    [SCREEN_ID_HOME] = create_screen_home,
    [SCREEN_ID_SETTINGS] = create_screen_settings,
    [SCREEN_ID_ROOM_LIST] = create_screen_room_list,
    [SCREEN_ID_HEART_LUXURY] = create_screen_heart_luxury,
};

// ============================================================================
// Public Functions
// ============================================================================

void ui_router_init(void)
{
    // Apply the global screen style to the default screen
    lv_obj_add_style(lv_scr_act(), &style_screen, 0);
    
    // Start with the home screen
    ui_router_go(SCREEN_ID_HOME);
}

void ui_router_go(screen_id_t screen_id)
{
    if (screen_id >= SCREEN_ID_COUNT) {
        LV_LOG_ERROR("Invalid screen ID: %d", screen_id);
        return;
    }

    // 1. Create the new screen
    lv_obj_t *new_screen = screen_create_funcs[screen_id]();
    if (!new_screen) {
        LV_LOG_ERROR("Failed to create screen ID: %d", screen_id);
        return;
    }

    // 2. Apply global screen style
    lv_obj_add_style(new_screen, &style_screen, 0);

    // 3. Transition to the new screen
    if (active_screen) {
        // Use the built-in transition utility
        ui_screen_transition(new_screen, UI_ANIM_NORMAL);
    } else {
        // First screen load, no animation needed
        lv_scr_load(new_screen);
    }

    // 4. Update the active screen reference
    active_screen = new_screen;
    LV_LOG_INFO("Navigated to screen ID: %d", screen_id);
}

lv_obj_t* ui_router_get_active_screen(void)
{
    return active_screen;
}
