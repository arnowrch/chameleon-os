/**
 * @file app_clock.cpp
 * @brief Clock app with time display
 * Sector 1: Productivity
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>
#include <esp_log.h>
#include <time.h>

static const char* TAG = "APP_CLOCK";

static void clock_init(void) {
    ESP_LOGI(TAG, "Clock app initialized");
}

static void clock_on_enter(void) {
    ESP_LOGI(TAG, "Entering clock app");
}

static void clock_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    
    // Get current time
    time_t now = ntp_hal_get_time();
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    
    // Draw time
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
    display_hal_text(90, 45, time_str, primary, 3);
    
    // Draw date
    char date_str[32];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", &timeinfo);
    display_hal_text(90, 85, date_str, fg, 1);
    
    // Sync status
    if (ntp_hal_is_synced()) {
        display_hal_text(120, 105, "Synced", COLOR_GREEN, 1);
    } else {
        display_hal_text(110, 105, "Not Synced", COLOR_YELLOW, 1);
    }
}

static void clock_on_exit(void) {
    ESP_LOGI(TAG, "Exiting clock app");
}

static void clock_cleanup(void) {
    ESP_LOGI(TAG, "Clock app cleanup");
}

App_t app_clock = {
    .name = "Clock",
    .icon = "clock",
    .sector = 1,
    .init = clock_init,
    .on_enter = clock_on_enter,
    .on_update = clock_on_update,
    .on_exit = clock_on_exit,
    .cleanup = clock_cleanup
};
