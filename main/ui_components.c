/**
 * @file ui_components.c
 * @brief Reusable UI Components Implementation
 */

#include "ui_components.h"

// ============================================================================
// Component: Tile
// ============================================================================

lv_obj_t* ui_make_tile(lv_obj_t *parent)
{
    lv_obj_t *tile = lv_obj_create(parent);
    lv_obj_set_size(tile, UI_TILE_WIDTH, UI_TILE_HEIGHT);
    lv_obj_add_style(tile, &style_tile, 0);
    lv_obj_clear_flag(tile, LV_OBJ_FLAG_SCROLLABLE);  // No scroll
    
    return tile;
}

// ============================================================================
// Component: Button
// ============================================================================

lv_obj_t* ui_make_button(lv_obj_t *parent, const char *text)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT);
    lv_obj_add_style(btn, &style_button, 0);
    
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    
    // Apply press animation
    ui_apply_press_anim(btn);
    
    return btn;
}

lv_obj_t* ui_make_button_colored(lv_obj_t *parent, const char *text, lv_color_t bg_color)
{
    lv_obj_t *btn = ui_make_button(parent, text);
    lv_obj_set_style_bg_color(btn, bg_color, 0);
    
    return btn;
}

// ============================================================================
// Component: Arc Gauge
// ============================================================================

lv_obj_t* ui_make_arc_gauge(lv_obj_t *parent, int16_t min_value, int16_t max_value, int16_t current)
{
    lv_obj_t *arc = lv_arc_create(parent);
    lv_obj_set_size(arc, 56, 56);
    
    // Configure range
    lv_arc_set_range(arc, min_value, max_value);
    lv_arc_set_value(arc, current);
    
    // Angles: 135° to 45° (270° arc, gap at bottom)
    lv_arc_set_bg_angles(arc, 135, 45);
    lv_arc_set_angles(arc, 135, 135);  // Start at 135°
    
    // Disable knob (just indicator, not interactive)
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    
    // Apply styles
    lv_obj_add_style(arc, &style_arc_background, LV_PART_MAIN);
    lv_obj_add_style(arc, &style_arc_indicator, LV_PART_INDICATOR);
    
    return arc;
}

void ui_arc_animate_value(lv_obj_t *arc, int16_t new_value, uint32_t duration)
{
    int16_t current = lv_arc_get_value(arc);
    
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, arc);
    lv_anim_set_values(&anim, current, new_value);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_path_cb(&anim, UI_ANIM_EASE_IN_OUT);
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_arc_set_value);
    lv_anim_start(&anim);
}

// ============================================================================
// Component: Toast Notification
// ============================================================================

static void toast_delete_cb(lv_anim_t *anim)
{
    lv_obj_t *toast = anim->var;
    lv_obj_del(toast);
}

lv_obj_t* ui_show_toast(lv_obj_t *parent, const char *text, const char *icon)
{
    lv_obj_t *toast = lv_obj_create(parent);
    // Use LV_PCT(100) for width to ensure it takes the full screen width (320px in landscape)
    lv_obj_set_size(toast, LV_PCT(100), UI_TOAST_HEIGHT);
    lv_obj_set_style_bg_color(toast, UI_COLOR_BG_TERTIARY, 0);
    lv_obj_set_style_bg_opa(toast, UI_OPA_90, 0);
    lv_obj_set_style_border_width(toast, 0, 0);
    lv_obj_set_style_radius(toast, UI_RADIUS_SM, 0);
    lv_obj_set_style_pad_all(toast, UI_SPACE_XS, 0);
    lv_obj_clear_flag(toast, LV_OBJ_FLAG_SCROLLABLE);
    
    // Position at bottom
    lv_obj_align(toast, LV_ALIGN_BOTTOM_MID, 0, -UI_SPACE_XS);
    
    // Create label with optional icon
    lv_obj_t *label = lv_label_create(toast);
    if (icon != NULL) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s %s", icon, text);
        lv_label_set_text(label, buf);
    } else {
        lv_label_set_text(label, text);
    }
    lv_obj_add_style(label, &style_text_body, 0);
    lv_obj_center(label);
    
    // Fade in
    ui_apply_fade_in(toast, 0, UI_ANIM_FAST);
    
    // Auto-dismiss after 4 seconds with fade out
    lv_anim_t anim_out;
    lv_anim_init(&anim_out);
    lv_anim_set_var(&anim_out, toast);
    lv_anim_set_values(&anim_out, UI_OPA_FULL, UI_OPA_INVISIBLE);
    lv_anim_set_time(&anim_out, UI_ANIM_NORMAL);
    lv_anim_set_delay(&anim_out, 4000);  // Wait 4s before fade out
    lv_anim_set_path_cb(&anim_out, UI_ANIM_EASE_IN_OUT);
    lv_anim_set_exec_cb(&anim_out, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
    lv_anim_set_deleted_cb(&anim_out, toast_delete_cb);
    lv_anim_start(&anim_out);
    
    return toast;
}

// ============================================================================
// Component: Power Button
// ============================================================================

lv_obj_t* ui_make_power_button(lv_obj_t *parent)
{
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, UI_POWER_BTN_SIZE, UI_POWER_BTN_SIZE);
    lv_obj_set_style_radius(btn, UI_RADIUS_ROUND, 0);  // Circular
    lv_obj_set_style_bg_color(btn, UI_COLOR_BG_TERTIARY, 0);
    
    // Power icon (using symbol)
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_POWER);
    lv_obj_set_style_text_font(label, UI_FONT_H1, 0);
    lv_obj_center(label);
    
    // Press animation
    ui_apply_press_anim(btn);
    
    return btn;
}

void ui_power_button_set_state(lv_obj_t *btn, bool is_on)
{
    lv_color_t target_color = is_on ? UI_COLOR_ACCENT_GREEN : UI_COLOR_BG_TERTIARY;
    ui_animate_color(btn, target_color, UI_ANIM_NORMAL);
}

// ============================================================================
// Component: Room List Item
// ============================================================================

lv_obj_t* ui_make_room_list_item(lv_obj_t *parent, const char *icon_symbol, 
                                  const char *room_name, bool has_chevron)
{
    lv_obj_t *item = lv_obj_create(parent);
    lv_obj_set_size(item, LV_PCT(100), UI_LIST_ITEM_HEIGHT);
    lv_obj_set_style_bg_color(item, UI_COLOR_BG_SECONDARY, 0);
    lv_obj_set_style_bg_opa(item, UI_OPA_FULL, 0);
    lv_obj_set_style_border_width(item, 0, 0);
    lv_obj_set_style_radius(item, UI_RADIUS_SM, 0);
    lv_obj_set_style_pad_all(item, UI_SPACE_SM, 0);
    lv_obj_clear_flag(item, LV_OBJ_FLAG_SCROLLABLE);
    
    // Icon
    lv_obj_t *icon = lv_label_create(item);
    lv_label_set_text(icon, icon_symbol);
    lv_obj_set_style_text_font(icon, UI_FONT_H2, 0);
    lv_obj_align(icon, LV_ALIGN_LEFT_MID, 0, 0);
    
    // Room name
    lv_obj_t *label = lv_label_create(item);
    lv_label_set_text(label, room_name);
    lv_obj_add_style(label, &style_text_body, 0);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, UI_LIST_ICON_SIZE + UI_SPACE_SM, 0);
    
    // Chevron arrow (optional)
    if (has_chevron) {
        lv_obj_t *chevron = lv_label_create(item);
        lv_label_set_text(chevron, LV_SYMBOL_RIGHT);
        lv_obj_set_style_text_color(chevron, UI_COLOR_TEXT_DISABLED, 0);
        lv_obj_align(chevron, LV_ALIGN_RIGHT_MID, 0, 0);
    }
    
    // Press animation
    ui_apply_press_anim(item);
    
    return item;
}

// ============================================================================
// Component: Status Badge
// ============================================================================

lv_obj_t* ui_make_status_badge(lv_obj_t *parent, const char *text, lv_color_t color)
{
    lv_obj_t *badge = lv_obj_create(parent);
    lv_obj_set_size(badge, LV_SIZE_CONTENT, 20);
    lv_obj_set_style_bg_color(badge, color, 0);
    lv_obj_set_style_bg_opa(badge, UI_OPA_30, 0);
    lv_obj_set_style_border_width(badge, 0, 0);
    lv_obj_set_style_radius(badge, UI_RADIUS_ROUND, 0);  // Pill shape
    lv_obj_set_style_pad_hor(badge, UI_SPACE_XS, 0);
    lv_obj_set_style_pad_ver(badge, 2, 0);
    lv_obj_clear_flag(badge, LV_OBJ_FLAG_SCROLLABLE);
    
    lv_obj_t *label = lv_label_create(badge);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, UI_FONT_BODY, 0);
    lv_obj_set_style_text_color(label, color, 0);
    lv_obj_center(label);
    
    return badge;
}

// ============================================================================
// Component: Slider
// ============================================================================

lv_obj_t* ui_make_slider(lv_obj_t *parent, int16_t min_value, int16_t max_value, 
                          int16_t current, const char *label_text)
{
    // Container
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, 140, 36);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    
    // Label
    lv_obj_t *label = lv_label_create(container);
    lv_label_set_text(label, label_text);
    lv_obj_add_style(label, &style_text_body, 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
    
    // Slider
    lv_obj_t *slider = lv_slider_create(container);
    lv_obj_set_width(slider, LV_PCT(100));
    lv_slider_set_range(slider, min_value, max_value);
    lv_slider_set_value(slider, current, LV_ANIM_OFF);
    lv_obj_align(slider, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    
    // Style slider
    lv_obj_set_style_bg_color(slider, UI_COLOR_BG_TERTIARY, LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, UI_COLOR_ACCENT_BLUE, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider, UI_COLOR_TEXT_PRIMARY, LV_PART_KNOB);
    lv_obj_set_style_pad_all(slider, 2, LV_PART_KNOB);
    
    return slider;
}

// ============================================================================
// Component: Icon Label
// ============================================================================

lv_obj_t* ui_make_icon_label(lv_obj_t *parent, const char *icon, const char *text)
{
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    
    // Icon
    lv_obj_t *icon_label = lv_label_create(container);
    lv_label_set_text(icon_label, icon);
    lv_obj_set_style_text_font(icon_label, UI_FONT_BODY, 0);
    lv_obj_set_style_text_color(icon_label, UI_COLOR_TEXT_SECONDARY, 0);
    
    // Text
    lv_obj_t *text_label = lv_label_create(container);
    lv_label_set_text(text_label, text);
    lv_obj_add_style(text_label, &style_text_body, 0);
    lv_obj_set_style_pad_left(text_label, UI_SPACE_XXS, 0);
    
    return container;
}

// ============================================================================
// Utility: Screen Transition
// ============================================================================

void ui_screen_transition(lv_obj_t *new_screen, uint32_t duration)
{
    lv_scr_load_anim(new_screen, LV_SCR_LOAD_ANIM_FADE_IN, duration, 0, false);
}