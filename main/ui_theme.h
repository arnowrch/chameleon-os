/**
 * @file ui_theme.h
 * @brief Luxury UI Theme - Color System & Styles
 * 
 * Design Philosophy: Tesla UI, Apple Home, Dyson
 * Hardware: Waveshare ESP32-S3-Touch-LCD-1.47 (172×320)
 */

#ifndef UI_THEME_H
#define UI_THEME_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Color Palette - Dark Theme
// ============================================================================

// Base Colors
#define UI_COLOR_BG_PRIMARY       lv_color_hex(0x0B0B0B)  // Main background
#define UI_COLOR_BG_SECONDARY     lv_color_hex(0x1A1A1A)  // Card/tile background
#define UI_COLOR_BG_TERTIARY      lv_color_hex(0x2A2A2A)  // Elevated surface

// Text Colors
#define UI_COLOR_TEXT_PRIMARY     lv_color_hex(0xFFFFFF)  // Main text (white)
#define UI_COLOR_TEXT_SECONDARY   lv_color_hex(0xB0B0B0)  // Secondary text (70% white)
#define UI_COLOR_TEXT_DISABLED    lv_color_hex(0x666666)  // Disabled state (40% white)

// Accent Colors
#define UI_COLOR_ACCENT_BLUE      lv_color_hex(0x007AFF)  // Interactive elements
#define UI_COLOR_ACCENT_GREEN     lv_color_hex(0x34C759)  // Success/active
#define UI_COLOR_ACCENT_ORANGE    lv_color_hex(0xFF9500)  // Warning
#define UI_COLOR_ACCENT_RED       lv_color_hex(0xFF3B30)  // Error/critical

// State Colors
#define UI_COLOR_STATE_HOVER      lv_color_hex(0x333333)  // Hover overlay
#define UI_COLOR_STATE_PRESSED    lv_color_hex(0x444444)  // Pressed state
#define UI_COLOR_STATE_FOCUS      lv_color_hex(0x007AFF)  // Focus ring
#define UI_COLOR_STATE_DISABLED   lv_color_hex(0x1A1A1A)  // Disabled background

// Opacity Values (0-255)
#define UI_OPA_FULL               255  // 100%
#define UI_OPA_90                 229  // 90%
#define UI_OPA_70                 179  // 70%
#define UI_OPA_50                 128  // 50%
#define UI_OPA_30                 77   // 30%
#define UI_OPA_10                 26   // 10%
#define UI_OPA_INVISIBLE          0    // 0%

// ============================================================================
// Layout System - 4px Base Grid
// ============================================================================

#define UI_UNIT                   4    // Base unit (4px)

// Spacing Scale
#define UI_SPACE_XXS              (UI_UNIT * 1)   // 4px
#define UI_SPACE_XS               (UI_UNIT * 2)   // 8px
#define UI_SPACE_SM               (UI_UNIT * 3)   // 12px
#define UI_SPACE_MD               (UI_UNIT * 4)   // 16px
#define UI_SPACE_LG               (UI_UNIT * 5)   // 20px
#define UI_SPACE_XL               (UI_UNIT * 6)   // 24px
#define UI_SPACE_XXL              (UI_UNIT * 8)   // 32px

// Screen Margins
#define UI_MARGIN_SCREEN          UI_SPACE_XS     // 8px side margins

// Component Dimensions
#define UI_TILE_WIDTH             148
#define UI_TILE_HEIGHT            68
#define UI_TILE_GAP               UI_SPACE_XS     // 8px between tiles

#define UI_BUTTON_WIDTH           148
#define UI_BUTTON_HEIGHT          36

#define UI_TOAST_WIDTH            320             // Full width (Landscape)
#define UI_TOAST_HEIGHT           28

#define UI_POWER_BTN_SIZE         64

#define UI_LIST_ITEM_HEIGHT       48
#define UI_LIST_ICON_SIZE         24

// Border Radius
#define UI_RADIUS_SM              (UI_UNIT * 2)   // 8px
#define UI_RADIUS_MD              (UI_UNIT * 3)   // 12px
#define UI_RADIUS_LG              (UI_UNIT * 4)   // 16px
#define UI_RADIUS_ROUND           9999            // Fully rounded (circle/pill)

// ============================================================================
// Typography - Montserrat Font Family
// ============================================================================

// Font Sizes (declare fonts in your project)
#define UI_FONT_SIZE_H1           22   // Primary heading
#define UI_FONT_SIZE_H2           18   // Secondary heading
#define UI_FONT_SIZE_BODY         12   // Body text

// Font Weights
#define UI_FONT_WEIGHT_LIGHT      300
#define UI_FONT_WEIGHT_REGULAR    400
#define UI_FONT_WEIGHT_MEDIUM     500
#define UI_FONT_WEIGHT_SEMIBOLD   600
#define UI_FONT_WEIGHT_BOLD       700

// Font References (link to your project's font assets)
extern const lv_font_t montserrat_22_bold;     // H1: 22px Bold
extern const lv_font_t montserrat_18_medium;   // H2: 18px Medium
extern const lv_font_t montserrat_12_regular;  // Body: 12px Regular

#define UI_FONT_H1                &montserrat_22_bold
#define UI_FONT_H2                &montserrat_18_medium
#define UI_FONT_BODY              &montserrat_12_regular

// ============================================================================
// Animation System
// ============================================================================

// Duration Scale (milliseconds)
#define UI_ANIM_INSTANT           0      // No animation
#define UI_ANIM_FAST              100    // Quick feedback
#define UI_ANIM_NORMAL            200    // Standard interactions
#define UI_ANIM_SLOW              400    // Emphasis/entrance
#define UI_ANIM_VERY_SLOW         600    // Complex transitions

// Easing Functions (LVGL built-in)
#define UI_ANIM_EASE_IN_OUT       lv_anim_path_ease_in_out
#define UI_ANIM_EASE_OUT          lv_anim_path_ease_out
#define UI_ANIM_LINEAR            lv_anim_path_linear
#define UI_ANIM_OVERSHOOT         lv_anim_path_overshoot  // Use sparingly!

// ============================================================================
// Style Presets
// ============================================================================

// Base Styles (call ui_theme_init() to set up)
extern lv_style_t style_screen;           // Full-screen background
extern lv_style_t style_tile;             // Card/tile surface
extern lv_style_t style_button;           // Interactive button
extern lv_style_t style_button_pressed;   // Button pressed state
extern lv_style_t style_text_h1;          // H1 text style
extern lv_style_t style_text_h2;          // H2 text style
extern lv_style_t style_text_body;        // Body text style
extern lv_style_t style_arc_indicator;    // Arc gauge indicator
extern lv_style_t style_arc_background;   // Arc gauge background

// ============================================================================
// Public Functions
// ============================================================================

/**
 * @brief Initialize the UI theme (call once at startup)
 * 
 * Sets up all global styles defined in this theme.
 * Must be called after lv_init() and before creating UI.
 * 
 * @example
 *   lv_init();
 *   ui_theme_init();
 *   // Now create your UI components
 */
void ui_theme_init(void);

/**
 * @brief Apply fade-in animation to an object
 * 
 * @param obj     Target LVGL object
 * @param delay   Delay before animation starts (ms)
 * @param duration Animation duration (ms)
 * 
 * @example
 *   lv_obj_t *tile = ui_make_tile(...);
 *   ui_apply_fade_in(tile, 0, UI_ANIM_NORMAL);
 */
void ui_apply_fade_in(lv_obj_t *obj, uint32_t delay, uint32_t duration);

/**
 * @brief Apply press feedback animation
 * 
 * Scales object down slightly when pressed, returns on release.
 * 
 * @param obj Target LVGL object (usually a button)
 * 
 * @example
 *   lv_obj_t *btn = ui_make_button(...);
 *   ui_apply_press_anim(btn);
 */
void ui_apply_press_anim(lv_obj_t *obj);

/**
 * @brief Create a smooth color transition animation
 * 
 * @param obj       Target LVGL object
 * @param color     Target color
 * @param duration  Animation duration (ms)
 * 
 * @example
 *   ui_animate_color(arc, UI_COLOR_ACCENT_GREEN, UI_ANIM_NORMAL);
 */
void ui_animate_color(lv_obj_t *obj, lv_color_t color, uint32_t duration);

#ifdef __cplusplus
}
#endif

#endif // UI_THEME_H