/**
 * @file app_wifi.cpp
 * @brief WiFi Manager app
 * Sector 2: Communication
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>
#include <esp_log.h>

static const char* TAG = "APP_WIFI";

static void wifi_init(void) {
    ESP_LOGI(TAG, "WiFi app initialized");
}

static void wifi_on_enter(void) {
    ESP_LOGI(TAG, "Entering WiFi manager");
}

static void wifi_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    
    display_hal_text(80, 35, "WiFi Manager", primary, 2);
    
    // Get WiFi status
    WiFiStatus_t status = wifi_hal_get_status();
    
    // Draw status
    display_hal_text(30, 60, "Status:", fg, 1);
    
    const char* status_str = "Unknown";
    uint16_t status_color = fg;
    
    switch (status) {
        case WIFI_STATUS_CONNECTED:
            status_str = "Connected";
            status_color = COLOR_GREEN;
            break;
        case WIFI_STATUS_CONNECTING:
            status_str = "Connecting...";
            status_color = COLOR_YELLOW;
            break;
        case WIFI_STATUS_DISCONNECTED:
            status_str = "Disconnected";
            status_color = COLOR_RED;
            break;
        case WIFI_STATUS_ERROR:
            status_str = "Error";
            status_color = COLOR_RED;
            break;
    }
    
    display_hal_text(100, 60, status_str, status_color, 1);
    
    // If connected, show IP and signal
    if (status == WIFI_STATUS_CONNECTED) {
        char ip_str[32];
        wifi_hal_get_ip(ip_str);
        
        char info[64];
        snprintf(info, sizeof(info), "IP: %s", ip_str);
        display_hal_text(30, 80, info, fg, 1);
        
        int8_t rssi = wifi_hal_get_rssi();
        snprintf(info, sizeof(info), "Signal: %d dBm", rssi);
        display_hal_text(30, 95, info, fg, 1);
    }
}

static void wifi_on_exit(void) {
    ESP_LOGI(TAG, "Exiting WiFi manager");
}

static void wifi_cleanup(void) {
    ESP_LOGI(TAG, "WiFi app cleanup");
}

App_t app_wifi = {
    .name = "WiFi",
    .icon = "wifi",
    .sector = 2,
    .init = wifi_init,
    .on_enter = wifi_on_enter,
    .on_update = wifi_on_update,
    .on_exit = wifi_on_exit,
    .cleanup = wifi_cleanup
};
