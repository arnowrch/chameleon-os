/**
 * @file ui_theme.c
 * @brief Luxury UI Theme Implementation
 */

#include "ui_theme.h"

// ============================================================================
// Global Style Definitions
// ============================================================================

lv_style_t style_screen;
lv_style_t style_tile;
lv_style_t style_button;
lv_style_t style_button_pressed;
lv_style_t style_text_h1;
lv_style_t style_text_h2;
lv_style_t style_text_body;
lv_style_t style_arc_indicator;
lv_style_t style_arc_background;

// ============================================================================
// Theme Initialization
// ============================================================================

void ui_theme_init(void)
{
    // -------------------------------------------------------------------------
    // Screen Style (Full Background)
    // -------------------------------------------------------------------------
    lv_style_init(&style_screen);
    lv_style_set_bg_color(&style_screen, UI_COLOR_BG_PRIMARY);
    lv_style_set_bg_opa(&style_screen, UI_OPA_FULL);
    lv_style_set_border_width(&style_screen, 0);
    lv_style_set_pad_all(&style_screen, 0);

    // -------------------------------------------------------------------------
    // Tile Style (Card/Surface)
    // -------------------------------------------------------------------------
    lv_style_init(&style_tile);
    lv_style_set_bg_color(&style_tile, UI_COLOR_BG_SECONDARY);
    lv_style_set_bg_opa(&style_tile, UI_OPA_FULL);
    lv_style_set_border_width(&style_tile, 0);
    lv_style_set_radius(&style_tile, UI_RADIUS_MD);
    lv_style_set_pad_all(&style_tile, UI_SPACE_SM);
    lv_style_set_shadow_width(&style_tile, 0);  // No shadows for performance

    // -------------------------------------------------------------------------
    // Button Style (Default State)
    // -------------------------------------------------------------------------
    lv_style_init(&style_button);
    lv_style_set_bg_color(&style_button, UI_COLOR_BG_TERTIARY);
    lv_style_set_bg_opa(&style_button, UI_OPA_FULL);
    lv_style_set_border_width(&style_button, 0);
    lv_style_set_radius(&style_button, UI_RADIUS_SM);
    lv_style_set_pad_ver(&style_button, UI_SPACE_XS);
    lv_style_set_pad_hor(&style_button, UI_SPACE_MD);
    lv_style_set_text_color(&style_button, UI_COLOR_TEXT_PRIMARY);
    lv_style_set_text_font(&style_button, UI_FONT_BODY);

    // -------------------------------------------------------------------------
    // Button Pressed State
    // -------------------------------------------------------------------------
    lv_style_init(&style_button_pressed);
    lv_style_set_bg_color(&style_button_pressed, UI_COLOR_STATE_PRESSED);
    lv_style_set_bg_opa(&style_button_pressed, UI_OPA_FULL);
    lv_style_set_transform_zoom(&style_button_pressed, 240);  // 95% scale (256 = 100%)

    // -------------------------------------------------------------------------
    // Text Styles
    // -------------------------------------------------------------------------
    lv_style_init(&style_text_h1);
    lv_style_set_text_color(&style_text_h1, UI_COLOR_TEXT_PRIMARY);
    lv_style_set_text_font(&style_text_h1, UI_FONT_H1);
    lv_style_set_text_letter_space(&style_text_h1, 0);

    lv_style_init(&style_text_h2);
    lv_style_set_text_color(&style_text_h2, UI_COLOR_TEXT_PRIMARY);
    lv_style_set_text_font(&style_text_h2, UI_FONT_H2);
    lv_style_set_text_letter_space(&style_text_h2, 0);

    lv_style_init(&style_text_body);
    lv_style_set_text_color(&style_text_body, UI_COLOR_TEXT_SECONDARY);
    lv_style_set_text_font(&style_text_body, UI_FONT_BODY);
    lv_style_set_text_letter_space(&style_text_body, 0);

    // -------------------------------------------------------------------------
    // Arc Gauge Styles
    // -------------------------------------------------------------------------
    lv_style_init(&style_arc_indicator);
    lv_style_set_arc_color(&style_arc_indicator, UI_COLOR_ACCENT_GREEN);
    lv_style_set_arc_width(&style_arc_indicator, 6);
    lv_style_set_arc_rounded(&style_arc_indicator, true);

    lv_style_init(&style_arc_background);
    lv_style_set_arc_color(&style_arc_background, UI_COLOR_BG_TERTIARY);
    lv_style_set_arc_width(&style_arc_background, 6);
    lv_style_set_arc_rounded(&style_arc_background, true);
}

// ============================================================================
// Animation Utilities
// ============================================================================

void ui_apply_fade_in(lv_obj_t *obj, uint32_t delay, uint32_t duration)
{
    lv_obj_set_style_opa(obj, UI_OPA_INVISIBLE, 0);  // Start invisible
    
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, UI_OPA_INVISIBLE, UI_OPA_FULL);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_path_cb(&anim, UI_ANIM_EASE_OUT);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    lv_anim_start(&anim);
}

void ui_apply_press_anim(lv_obj_t *obj)
{
    // Add pressed state style to button
    lv_obj_add_style(obj, &style_button_pressed, LV_STATE_PRESSED);
    
    // Enable transition for smooth scale animation
    static lv_style_transition_dsc_t trans;
    static lv_style_prop_t props[] = {LV_STYLE_TRANSFORM_ZOOM, 0};
    lv_style_transition_dsc_init(&trans, props, UI_ANIM_EASE_OUT, UI_ANIM_FAST, 0, NULL);
    lv_obj_set_style_transition(obj, &trans, LV_STATE_PRESSED);
}

void ui_animate_color(lv_obj_t *obj, lv_color_t color, uint32_t duration)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_path_cb(&anim, UI_ANIM_EASE_IN_OUT);
    
    // For arc indicators (change color smoothly)
    if (lv_obj_check_type(obj, &lv_arc_class)) {
        lv_color_t current_color = lv_obj_get_style_arc_color(obj, LV_PART_INDICATOR);
        lv_anim_set_values(&anim, lv_color_to_u32(current_color), lv_color_to_u32(color));
        lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_arc_color);
        lv_anim_set_custom_exec_cb(&anim, NULL);  // Use default color interpolation
        lv_anim_start(&anim);
    }
    // For other objects (background color)
    else {
        lv_color_t current_color = lv_obj_get_style_bg_color(obj, 0);
        lv_anim_set_values(&anim, lv_color_to_u32(current_color), lv_color_to_u32(color));
        lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_bg_color);
        lv_anim_start(&anim);
    }
}

// ============================================================================
// Helper: Apply standard transitions to object
// ============================================================================

void ui_apply_standard_transition(lv_obj_t *obj)
{
    static lv_style_transition_dsc_t trans;
    static lv_style_prop_t props[] = {
        LV_STYLE_BG_COLOR,
        LV_STYLE_TRANSFORM_ZOOM,
        0
    };
    lv_style_transition_dsc_init(&trans, props, UI_ANIM_EASE_OUT, UI_ANIM_NORMAL, 0, NULL);
    lv_obj_set_style_transition(obj, &trans, 0);
}