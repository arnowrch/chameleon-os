/**
 * @file ui_components.h
 * @brief Reusable UI Components for Luxury Interface
 * 
 * Pre-built components following the Design System specifications.
 * All components are LVGL objects with applied theme styles.
 */

#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "lvgl.h"
#include "ui_theme.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Component: Tile (Card/Surface)
// ============================================================================

/**
 * @brief Create a luxury tile (card) component
 * 
 * Specs: 156×72px, rounded corners (12px), dark surface (#1A1A1A)
 * 
 * @param parent  Parent container (or NULL for screen)
 * @return        Tile object (lv_obj_t*)
 * 
 * @example
 *   lv_obj_t *tile = ui_make_tile(parent);
 *   lv_obj_align(tile, LV_ALIGN_TOP_LEFT, 8, 8);
 *   lv_obj_t *label = lv_label_create(tile);
 *   lv_label_set_text(label, "Living Room");
 */
lv_obj_t* ui_make_tile(lv_obj_t *parent);

// ============================================================================
// Component: Button
// ============================================================================

/**
 * @brief Create a luxury button with text
 * 
 * Specs: 156×36px, rounded corners (8px), hover/press animations
 * 
 * @param parent  Parent container
 * @param text    Button label text
 * @return        Button object (lv_obj_t*)
 * 
 * @example
 *   lv_obj_t *btn = ui_make_button(parent, "Turn On");
 *   lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);
 */
lv_obj_t* ui_make_button(lv_obj_t *parent, const char *text);

/**
 * @brief Create a button with custom color (for primary actions)
 * 
 * @param parent      Parent container
 * @param text        Button label text
 * @param bg_color    Background color (e.g., UI_COLOR_ACCENT_BLUE)
 * @return            Button object (lv_obj_t*)
 * 
 * @example
 *   lv_obj_t *btn = ui_make_button_colored(parent, "Save", UI_COLOR_ACCENT_GREEN);
 */
lv_obj_t* ui_make_button_colored(lv_obj_t *parent, const char *text, lv_color_t bg_color);

// ============================================================================
// Component: Arc Gauge (Progress/Status Indicator)
// ============================================================================

/**
 * @brief Create a circular arc gauge
 * 
 * Specs: 56×56px, 6px line width, 270° arc (from 135° to 45°)
 * 
 * @param parent      Parent container
 * @param min_value   Minimum value (e.g., 0)
 * @param max_value   Maximum value (e.g., 100)
 * @param current     Current value (e.g., 72)
 * @return            Arc object (lv_arc_t*)
 * 
 * @example
 *   lv_obj_t *arc = ui_make_arc_gauge(tile, 0, 100, 72);
 *   lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
 *   // Update later: lv_arc_set_value(arc, 85);
 */
lv_obj_t* ui_make_arc_gauge(lv_obj_t *parent, int16_t min_value, int16_t max_value, int16_t current);

/**
 * @brief Animate arc gauge value change
 * 
 * @param arc         Arc gauge object
 * @param new_value   Target value
 * @param duration    Animation duration (ms) - use UI_ANIM_NORMAL (200ms)
 * 
 * @example
 *   ui_arc_animate_value(arc, 90, UI_ANIM_NORMAL);
 */
void ui_arc_animate_value(lv_obj_t *arc, int16_t new_value, uint32_t duration);

// ============================================================================
// Component: Toast Notification
// ============================================================================

/**
 * @brief Show a temporary toast notification at screen bottom
 * 
 * Specs: 172×28px (full width), 4s duration, auto-dismiss
 * 
 * @param parent  Parent screen
 * @param text    Toast message text
 * @param icon    Icon symbol (e.g., LV_SYMBOL_OK, LV_SYMBOL_WARNING) or NULL
 * @return        Toast object (lv_obj_t*)
 * 
 * @example
 *   ui_show_toast(lv_scr_act(), "Light turned on", LV_SYMBOL_OK);
 */
lv_obj_t* ui_show_toast(lv_obj_t *parent, const char *text, const char *icon);

// ============================================================================
// Component: Power Button (Round Toggle)
// ============================================================================

/**
 * @brief Create a round power button (64×64px)
 * 
 * Specs: Circle button with power icon, toggle state with color change
 * 
 * @param parent  Parent container
 * @return        Button object (lv_obj_t*) - Use LV_EVENT_CLICKED to handle
 * 
 * @example
 *   lv_obj_t *power_btn = ui_make_power_button(tile);
 *   lv_obj_add_event_cb(power_btn, power_toggle_cb, LV_EVENT_CLICKED, NULL);
 */
lv_obj_t* ui_make_power_button(lv_obj_t *parent);

/**
 * @brief Set power button state (on/off with animation)
 * 
 * @param btn     Power button object
 * @param is_on   true = ON (green), false = OFF (gray)
 * 
 * @example
 *   ui_power_button_set_state(power_btn, true);  // Turn on (green)
 */
void ui_power_button_set_state(lv_obj_t *btn, bool is_on);

// ============================================================================
// Component: Room List Item
// ============================================================================

/**
 * @brief Create a list item for room selection
 * 
 * Specs: 172×48px, icon (24×24px) + text + chevron, tap feedback
 * 
 * @param parent      Parent list container
 * @param icon_symbol Icon (e.g., LV_SYMBOL_HOME, LV_SYMBOL_BATTERY)
 * @param room_name   Room name text
 * @param has_chevron Show right chevron arrow (true/false)
 * @return            List item object (lv_obj_t*)
 * 
 * @example
 *   lv_obj_t *list = lv_list_create(parent);
 *   ui_make_room_list_item(list, LV_SYMBOL_HOME, "Living Room", true);
 *   ui_make_room_list_item(list, LV_SYMBOL_CHARGE, "Bedroom", true);
 */
lv_obj_t* ui_make_room_list_item(lv_obj_t *parent, const char *icon_symbol, 
                                  const char *room_name, bool has_chevron);

// ============================================================================
// Component: Status Badge
// ============================================================================

/**
 * @brief Create a small status badge (e.g., "3 devices")
 * 
 * Specs: Auto-width × 20px, rounded pill shape, small font
 * 
 * @param parent  Parent container
 * @param text    Badge text (keep short)
 * @param color   Badge color (e.g., UI_COLOR_ACCENT_BLUE)
 * @return        Badge object (lv_obj_t*)
 * 
 * @example
 *   lv_obj_t *badge = ui_make_status_badge(tile, "3 devices", UI_COLOR_ACCENT_BLUE);
 *   lv_obj_align(badge, LV_ALIGN_TOP_RIGHT, -8, 8);
 */
lv_obj_t* ui_make_status_badge(lv_obj_t *parent, const char *text, lv_color_t color);

// ============================================================================
// Component: Slider (Brightness/Volume Control)
// ============================================================================

/**
 * @brief Create a horizontal slider with value label
 * 
 * Specs: 140×36px (with label), smooth thumb animation
 * 
 * @param parent      Parent container
 * @param min_value   Minimum value (e.g., 0)
 * @param max_value   Maximum value (e.g., 100)
 * @param current     Current value
 * @param label_text  Label text (e.g., "Brightness")
 * @return            Slider object (lv_slider_t*)
 * 
 * @example
 *   lv_obj_t *slider = ui_make_slider(tile, 0, 100, 75, "Brightness");
 *   lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
 */
lv_obj_t* ui_make_slider(lv_obj_t *parent, int16_t min_value, int16_t max_value, 
                          int16_t current, const char *label_text);

// ============================================================================
// Component: Icon Label (Icon + Text Combo)
// ============================================================================

/**
 * @brief Create an icon+text label (for temperature, status, etc.)
 * 
 * @param parent  Parent container
 * @param icon    Icon symbol (e.g., LV_SYMBOL_CHARGE)
 * @param text    Text to display next to icon
 * @return        Container object (lv_obj_t*) with icon+label inside
 * 
 * @example
 *   lv_obj_t *temp = ui_make_icon_label(tile, LV_SYMBOL_CHARGE, "72%");
 *   lv_obj_align(temp, LV_ALIGN_BOTTOM_LEFT, 12, -12);
 */
lv_obj_t* ui_make_icon_label(lv_obj_t *parent, const char *icon, const char *text);

// ============================================================================
// Utility: Screen Transition
// ============================================================================

/**
 * @brief Switch to a new screen with fade animation
 * 
 * @param new_screen  New screen object (created with lv_obj_create(NULL))
 * @param duration    Fade duration (ms) - use UI_ANIM_NORMAL (200ms)
 * 
 * @example
 *   lv_obj_t *settings_screen = lv_obj_create(NULL);
 *   // ... populate settings_screen ...
 *   ui_screen_transition(settings_screen, UI_ANIM_NORMAL);
 */
void ui_screen_transition(lv_obj_t *new_screen, uint32_t duration);

#ifdef __cplusplus
}
#endif

#endif // UI_COMPONENTS_H