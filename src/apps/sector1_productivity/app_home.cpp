/**
 * @file app_home.cpp
 * @brief Home/Dashboard app - default startup app
 * Sector 1: Productivity
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>
#include <esp_log.h>

static const char* TAG = "APP_HOME";

static void home_init(void) {
    ESP_LOGI(TAG, "Home app initialized");
}

static void home_on_enter(void) {
    ESP_LOGI(TAG, "Entering home screen");
}

static void home_on_update(void) {
    // Draw home screen content
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    
    // Draw welcome message
    display_hal_text(60, 40, "Chameleon OS", primary, 2);
    display_hal_text(50, 70, "Universal Embedded OS", fg, 1);
    display_hal_text(40, 90, "33 Apps in One Device", fg, 1);
    
    // Draw system info
    char info[64];
    snprintf(info, sizeof(info), "Frame: %lu", display_hal_get_frame_count());
    display_hal_text(10, 110, info, fg, 1);
}

static void home_on_exit(void) {
    ESP_LOGI(TAG, "Exiting home screen");
}

static void home_cleanup(void) {
    ESP_LOGI(TAG, "Home app cleanup");
}

// App definition
App_t app_home = {
    .name = "Home",
    .icon = "home",
    .sector = 1,
    .init = home_init,
    .on_enter = home_on_enter,
    .on_update = home_on_update,
    .on_exit = home_on_exit,
    .cleanup = home_cleanup
};
