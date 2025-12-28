/**
 * @file theme_manager.cpp
 * @brief Theme Manager implementation
 */

#include "theme_manager.h"
#include "event_bus.h"
#include "hal.h"
#include <Arduino.h>
#include <esp_log.h>

static const char* TAG = "THEME_MGR";

// Current theme
static ThemeType_t current_theme = THEME_DARK;
static bool theme_initialized = false;

// Theme color palettes
static uint16_t theme_colors[3][THEME_COLOR_COUNT] = {
    // THEME_DARK
    {
        0x0000,  // BACKGROUND - Black
        0xFFFF,  // FOREGROUND - White
        0x1C9F,  // PRIMARY - Blue
        0x7BEF,  // SECONDARY - Light Blue
        0xFD20,  // ACCENT - Orange
        0x07E0,  // SUCCESS - Green
        0xFFE0,  // WARNING - Yellow
        0xF800,  // ERROR - Red
        0xFFFF,  // TEXT - White
        0xBDF7,  // TEXT_SECONDARY - Light Gray
        0x528A,  // BORDER - Dark Gray
        0x2104,  // SHADOW - Very Dark Gray
    },
    // THEME_LIGHT
    {
        0xFFFF,  // BACKGROUND - White
        0x0000,  // FOREGROUND - Black
        0x1C9F,  // PRIMARY - Blue
        0x5D7F,  // SECONDARY - Medium Blue
        0xFD20,  // ACCENT - Orange
        0x07E0,  // SUCCESS - Green
        0xFFE0,  // WARNING - Yellow
        0xF800,  // ERROR - Red
        0x0000,  // TEXT - Black
        0x6B4D,  // TEXT_SECONDARY - Dark Gray
        0xCE59,  // BORDER - Light Gray
        0xBDF7,  // SHADOW - Very Light Gray
    },
    // THEME_CUSTOM (starts as dark theme copy)
    {
        0x0000,  // BACKGROUND
        0xFFFF,  // FOREGROUND
        0x1C9F,  // PRIMARY
        0x7BEF,  // SECONDARY
        0xFD20,  // ACCENT
        0x07E0,  // SUCCESS
        0xFFE0,  // WARNING
        0xF800,  // ERROR
        0xFFFF,  // TEXT
        0xBDF7,  // TEXT_SECONDARY
        0x528A,  // BORDER
        0x2104,  // SHADOW
    }
};

/**
 * @brief Initialize theme manager
 */
int theme_manager_init(void) {
    ESP_LOGI(TAG, "Initializing theme manager...");
    
    // Set default theme
    current_theme = THEME_DARK;
    
    theme_initialized = true;
    ESP_LOGI(TAG, "Theme manager initialized (default: DARK)");
    return 0;
}

/**
 * @brief Set active theme
 */
int theme_set(ThemeType_t theme) {
    if (!theme_initialized) {
        ESP_LOGE(TAG, "Theme manager not initialized");
        return -1;
    }
    
    if (theme >= 3) {
        ESP_LOGE(TAG, "Invalid theme: %d", theme);
        return -2;
    }
    
    ESP_LOGI(TAG, "Switching theme: %d -> %d", current_theme, theme);
    current_theme = theme;
    
    // Publish theme change event
    Event_t event = {
        .type = EVENT_THEME_CHANGE,
        .data = &current_theme,
        .size = sizeof(current_theme),
        .timestamp = 0
    };
    event_bus_publish(&event);
    
    return 0;
}

/**
 * @brief Get current theme
 */
ThemeType_t theme_get(void) {
    return current_theme;
}

/**
 * @brief Get color for theme element
 */
uint16_t theme_get_color(ThemeColorId_t color_id) {
    if (!theme_initialized) {
        ESP_LOGW(TAG, "Theme manager not initialized, using defaults");
        return COLOR_WHITE;
    }
    
    if (color_id >= THEME_COLOR_COUNT) {
        ESP_LOGW(TAG, "Invalid color ID: %d", color_id);
        return COLOR_WHITE;
    }
    
    return theme_colors[current_theme][color_id];
}

/**
 * @brief Set custom color
 */
int theme_set_custom_color(ThemeColorId_t color_id, uint16_t color) {
    if (!theme_initialized) {
        ESP_LOGE(TAG, "Theme manager not initialized");
        return -1;
    }
    
    if (color_id >= THEME_COLOR_COUNT) {
        ESP_LOGE(TAG, "Invalid color ID: %d", color_id);
        return -2;
    }
    
    // Set in custom theme palette
    theme_colors[THEME_CUSTOM][color_id] = color;
    
    // If custom theme is active, publish change event
    if (current_theme == THEME_CUSTOM) {
        Event_t event = {
            .type = EVENT_THEME_CHANGE,
            .data = &current_theme,
            .size = sizeof(current_theme),
            .timestamp = 0
        };
        event_bus_publish(&event);
    }
    
    return 0;
}
