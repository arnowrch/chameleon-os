/**
 * @file lvgl_ui.c
 * @brief Main UI Initialization and Screen Implementations
 */

#include "lvgl_ui.h"
#include "ui_theme.h"
#include "ui_components.h"
#include "ui_router.h"

// Font definitions are in ui_fonts.c

// ============================================================================
// Main UI Initialization
// ============================================================================

void lvgl_ui_init(void)
{
    // 1. Initialize the UI Theme (Styles, Colors, Animations)
    ui_theme_init();
    
    // 2. Initialize the Screen Router and load the first screen (Home)
    ui_router_init();
}

// ============================================================================
// Screen Implementations
// ============================================================================

/**
 * @brief Event handler for the Home Screen's settings button.
 */
static void settings_btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_router_go(SCREEN_ID_SETTINGS);
        ui_show_toast(lv_scr_act(), "Settings opened", LV_SYMBOL_SETTINGS);
    }
}

/**
 * @brief Event handler for the Home Screen's room list button.
 */
static void room_list_btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_router_go(SCREEN_ID_ROOM_LIST);
        ui_show_toast(lv_scr_act(), "Room List opened", LV_SYMBOL_LIST);
    }
}

/**
 * @brief Event handler for the Home Screen's heart luxury button.
 */
static void heart_luxury_btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_router_go(SCREEN_ID_HEART_LUXURY);
        ui_show_toast(lv_scr_act(), "Luxury Demo opened", LV_SYMBOL_HEART);
    }
}

lv_obj_t* create_screen_home(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_all(screen, UI_MARGIN_SCREEN, 0);
    lv_obj_set_style_pad_row(screen, UI_TILE_GAP, 0);
    lv_obj_set_style_pad_column(screen, UI_TILE_GAP, 0);

    // Header
    lv_obj_t *header = lv_obj_create(screen);
    lv_obj_set_width(header, LV_PCT(100));
    lv_obj_set_height(header, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(header, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(header, 0, 0);
    lv_obj_set_style_pad_all(header, 0, 0);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    lv_obj_t *title = lv_label_create(header);
    lv_label_set_text(title, "Dashboard");
    lv_obj_add_style(title, &style_text_h1, 0);

    lv_obj_t *settings_btn = ui_make_button(header, LV_SYMBOL_SETTINGS);
    lv_obj_set_size(settings_btn, 36, 36);
    lv_obj_set_style_radius(settings_btn, UI_RADIUS_ROUND, 0);
    lv_obj_add_event_cb(settings_btn, settings_btn_event_cb, LV_EVENT_CLICKED, NULL);

    // Content Grid (2x2)
    lv_obj_t *grid = lv_obj_create(screen);
    lv_obj_set_width(grid, LV_PCT(100));
    lv_obj_set_height(grid, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(grid, 0, 0);
    lv_obj_set_style_pad_all(grid, 0, 0);
    lv_obj_set_layout(grid, &lv_grid_layout_dsc);
    // Grid for 320x172 (Landscape)
    // Layout: 2x2 Grid
    // Total width: 320 - 2*8 (margin) = 304
    // 2 Tiles + 1 Gap = 2*148 + 8 = 304
    lv_obj_set_style_grid_row_dsc_array(grid, (const lv_coord_t[]){UI_TILE_HEIGHT, UI_TILE_HEIGHT, LV_GRID_TEMPLATE_LAST}, 0);
    lv_obj_set_style_grid_column_dsc_array(grid, (const lv_coord_t[]){UI_TILE_WIDTH, UI_TILE_WIDTH, LV_GRID_TEMPLATE_LAST}, 0);
    lv_obj_set_style_grid_row_gap(grid, UI_TILE_GAP, 0);
    lv_obj_set_style_grid_column_gap(grid, UI_TILE_GAP, 0);

    // Tile 1: Light Control
    lv_obj_t *tile1 = ui_make_tile(grid);
    lv_obj_set_grid_cell(tile1, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_set_flex_flow(tile1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(tile1, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    
    lv_obj_t *tile1_title = lv_label_create(tile1);
    lv_label_set_text(tile1_title, "Living Room Light");
    lv_obj_add_style(tile1_title, &style_text_h2, 0);

    lv_obj_t *power_btn = ui_make_power_button(tile1);
    lv_obj_set_size(power_btn, 40, 40); // Smaller power button for tile
    lv_obj_set_style_pad_all(power_btn, 0, 0);
    lv_obj_align(power_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    ui_power_button_set_state(power_btn, true); // Initial state ON

    // Tile 2: Temperature Gauge
    lv_obj_t *tile2 = ui_make_tile(grid);
    lv_obj_set_grid_cell(tile2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    
    lv_obj_t *arc = ui_make_arc_gauge(tile2, 10, 30, 22);
    lv_obj_center(arc);
    lv_obj_t *temp_label = lv_label_create(arc);
    lv_label_set_text(temp_label, "22°C");
    lv_obj_add_style(temp_label, &style_text_h2, 0);
    lv_obj_center(temp_label);

    // Tile 3: Room List Navigation
    lv_obj_t *tile3 = ui_make_tile(grid);
    lv_obj_set_grid_cell(tile3, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(tile3, room_list_btn_event_cb, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *tile3_title = lv_label_create(tile3);
    lv_label_set_text(tile3_title, "Rooms");
    lv_obj_add_style(tile3_title, &style_text_h2, 0);

    lv_obj_t *badge = ui_make_status_badge(tile3, "3 Active", UI_COLOR_ACCENT_BLUE);
    lv_obj_align(badge, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    // Tile 4: Luxury Demo Navigation
    lv_obj_t *tile4 = ui_make_tile(grid);
    lv_obj_set_grid_cell(tile4, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(tile4, heart_luxury_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *tile4_title = lv_label_create(tile4);
    lv_label_set_text(tile4_title, "Luxury Demo");
    lv_obj_add_style(tile4_title, &style_text_h2, 0);
    
    return screen;
}

lv_obj_t* create_screen_settings(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen, UI_MARGIN_SCREEN, 0);
    lv_obj_set_style_pad_row(screen, UI_SPACE_XS, 0);

    // Header
    lv_obj_t *header = lv_obj_create(screen);
    lv_obj_set_width(header, LV_PCT(100));
    lv_obj_set_height(header, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(header, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(header, 0, 0);
    lv_obj_set_style_pad_all(header, 0, 0);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *back_btn = ui_make_button(header, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_btn, 36, 36);
    lv_obj_set_style_radius(back_btn, UI_RADIUS_ROUND, 0);
    lv_obj_add_event_cb(back_btn, settings_btn_event_cb, LV_EVENT_CLICKED, NULL); // Re-use handler to go back home

    lv_obj_t *title = lv_label_create(header);
    lv_label_set_text(title, "Settings");
    lv_obj_add_style(title, &style_text_h1, 0);
    lv_obj_set_style_pad_left(title, UI_SPACE_SM, 0);

    // Content: Slider example
    lv_obj_t *slider_tile = ui_make_tile(screen);
    lv_obj_set_width(slider_tile, LV_PCT(100));
    lv_obj_set_height(slider_tile, 80);
    lv_obj_set_flex_flow(slider_tile, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(slider_tile, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    ui_make_slider(slider_tile, 0, 100, 50, "Display Brightness");

    return screen;
}

lv_obj_t* create_screen_room_list(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen, UI_MARGIN_SCREEN, 0);
    lv_obj_set_style_pad_row(screen, UI_SPACE_XS, 0);

    // Header
    lv_obj_t *header = lv_obj_create(screen);
    lv_obj_set_width(header, LV_PCT(100));
    lv_obj_set_height(header, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(header, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(header, 0, 0);
    lv_obj_set_style_pad_all(header, 0, 0);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *back_btn = ui_make_button(header, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_btn, 36, 36);
    lv_obj_set_style_radius(back_btn, UI_RADIUS_ROUND, 0);
    lv_obj_add_event_cb(back_btn, room_list_btn_event_cb, LV_EVENT_CLICKED, NULL); // Re-use handler to go back home

    lv_obj_t *title = lv_label_create(header);
    lv_label_set_text(title, "Rooms");
    lv_obj_add_style(title, &style_text_h1, 0);
    lv_obj_set_style_pad_left(title, UI_SPACE_SM, 0);

    // List Container
    lv_obj_t *list_cont = lv_obj_create(screen);
    lv_obj_set_width(list_cont, LV_PCT(100));
    lv_obj_set_height(list_cont, LV_PCT(80));
    lv_obj_set_style_bg_opa(list_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(list_cont, 0, 0);
    lv_obj_set_style_pad_all(list_cont, 0, 0);
    lv_obj_set_flex_flow(list_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(list_cont, UI_SPACE_XS, 0);

    ui_make_room_list_item(list_cont, LV_SYMBOL_HOME, "Living Room", true);
    ui_make_room_list_item(list_cont, LV_SYMBOL_BED, "Master Bedroom", true);
    ui_make_room_list_item(list_cont, LV_SYMBOL_DRIVE, "Garage", true);
    ui_make_room_list_item(list_cont, LV_SYMBOL_WIFI, "Network Status", true);

    return screen;
}

lv_obj_t* create_screen_heart_luxury(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen, UI_MARGIN_SCREEN, 0);
    lv_obj_set_style_pad_row(screen, UI_SPACE_XS, 0);

    // Header
    lv_obj_t *header = lv_obj_create(screen);
    lv_obj_set_width(header, LV_PCT(100));
    lv_obj_set_height(header, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(header, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(header, 0, 0);
    lv_obj_set_style_pad_all(header, 0, 0);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(header, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *back_btn = ui_make_button(header, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_btn, 36, 36);
    lv_obj_set_style_radius(back_btn, UI_RADIUS_ROUND, 0);
    lv_obj_add_event_cb(back_btn, heart_luxury_btn_event_cb, LV_EVENT_CLICKED, NULL); // Re-use handler to go back home

    lv_obj_t *title = lv_label_create(header);
    lv_label_set_text(title, "Luxury Demo");
    lv_obj_add_style(title, &style_text_h1, 0);
    lv_obj_set_style_pad_left(title, UI_SPACE_SM, 0);

    // Content: Arc Gauge with animation
    lv_obj_t *tile = ui_make_tile(screen);
    lv_obj_set_width(tile, LV_PCT(100));
    lv_obj_set_height(tile, 150);
    lv_obj_set_flex_flow(tile, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(tile, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *arc = ui_make_arc_gauge(tile, 0, 100, 50);
    lv_obj_set_size(arc, 100, 100); // Adjusted arc size for landscape

    lv_obj_t *value_label = lv_label_create(arc);
    lv_label_set_text(value_label, "50%");
    lv_obj_add_style(value_label, &style_text_h1, 0);
    lv_obj_center(value_label);

    // Button to trigger animation
    lv_obj_t *anim_btn = ui_make_button_colored(screen, "Animate to 90%", UI_COLOR_ACCENT_ORANGE);
    lv_obj_set_width(anim_btn, LV_PCT(100));
    lv_obj_add_event_cb(anim_btn, [](lv_event_t *e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            lv_obj_t *arc_obj = (lv_obj_t *)lv_event_get_user_data(e);
            int16_t current_val = lv_arc_get_value(arc_obj);
            int16_t target_val = (current_val > 80) ? 20 : 90;
            lv_color_t target_color = (target_val > 80) ? UI_COLOR_ACCENT_GREEN : UI_COLOR_ACCENT_BLUE;

            ui_arc_animate_value(arc_obj, target_val, UI_ANIM_SLOW);
            ui_animate_color(lv_arc_get_indicator(arc_obj), target_color, UI_ANIM_SLOW);
            
            // Update label during animation (optional, for real-time feedback)
            // For simplicity, we just update the text here
            char buf[8];
            snprintf(buf, sizeof(buf), "%d%%", target_val);
            lv_label_set_text(lv_obj_get_child(arc_obj, 0), buf);

            ui_show_toast(lv_scr_act(), "Value animated", LV_SYMBOL_OK);
        }
    }, LV_EVENT_CLICKED, arc);

    return screen;
}
