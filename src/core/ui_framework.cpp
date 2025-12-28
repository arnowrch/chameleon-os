/**
 * @file ui_framework.cpp
 * @brief Focus-Carousel UI implementation
 */

#include "ui_framework.h"
#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>
#include <esp_log.h>

static const char* TAG = "UI_FW";

// Touch state for swipe detection
static int touch_start_x = 0;
static bool touch_active = false;

/**
 * @brief Initialize UI framework
 */
int ui_framework_init(void) {
    ESP_LOGI(TAG, "Initializing UI framework...");
    
    touch_start_x = 0;
    touch_active = false;
    
    ESP_LOGI(TAG, "Focus-Carousel UI initialized");
    return 0;
}

/**
 * @brief Navigate to previous app
 */
void ui_navigate_prev(void) {
    int current = app_get_current();
    int count = app_get_count();
    
    if (count == 0) return;
    
    int prev = (current - 1 + count) % count;
    app_switch(prev);
}

/**
 * @brief Navigate to next app
 */
void ui_navigate_next(void) {
    int current = app_get_current();
    int count = app_get_count();
    
    if (count == 0) return;
    
    int next = (current + 1) % count;
    app_switch(next);
}

/**
 * @brief Render UI
 */
void ui_render(void) {
    // Get theme colors
    uint16_t bg = theme_get_color(THEME_COLOR_BACKGROUND);
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    
    // Clear screen
    display_hal_clear(bg);
    
    // Draw status bar at top
    display_hal_rect(0, 0, DISPLAY_WIDTH, 20, primary);
    
    // Get current app
    int current_id = app_get_current();
    if (current_id >= 0) {
        App_t* app = app_get(current_id);
        if (app) {
            // Draw app name
            display_hal_text(10, 5, app->name, fg, 1);
            
            // Draw app counter
            char counter[16];
            snprintf(counter, sizeof(counter), "%d/%d", current_id + 1, app_get_count());
            display_hal_text(DISPLAY_WIDTH - 60, 5, counter, fg, 1);
        }
    }
    
    // Draw focus indicator (carousel dots)
    int dot_y = EFFECTIVE_HEIGHT - 10;
    int total_apps = app_get_count();
    if (total_apps > 0) {
        int dot_spacing = 12;
        int total_width = total_apps * dot_spacing;
        int start_x = (DISPLAY_WIDTH - total_width) / 2;
        
        for (int i = 0; i < total_apps && i < 20; i++) {  // Limit to 20 dots
            int x = start_x + i * dot_spacing;
            uint16_t color = (i == current_id) ? primary : fg;
            int size = (i == current_id) ? 3 : 2;
            
            // Draw dot
            for (int dy = -size; dy <= size; dy++) {
                for (int dx = -size; dx <= size; dx++) {
                    if (dx*dx + dy*dy <= size*size) {
                        display_hal_pixel(x + dx, dot_y + dy, color);
                    }
                }
            }
        }
    }
    
    // Swap buffers to display
    display_hal_swap();
}

/**
 * @brief Process touch input for swipe gestures
 */
void ui_process_touch(int x, int y, bool pressed) {
    if (pressed && !touch_active) {
        // Touch started
        touch_start_x = x;
        touch_active = true;
    } else if (!pressed && touch_active) {
        // Touch ended - check for swipe
        int delta = x - touch_start_x;
        
        if (abs(delta) > 50) {  // Swipe threshold
            if (delta > 0) {
                ESP_LOGI(TAG, "Swipe right - previous app");
                ui_navigate_prev();
            } else {
                ESP_LOGI(TAG, "Swipe left - next app");
                ui_navigate_next();
            }
        }
        
        touch_active = false;
    }
}
