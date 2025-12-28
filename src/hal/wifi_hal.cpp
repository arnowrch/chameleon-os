/**
 * @file wifi_hal.cpp
 * @brief WiFi HAL implementation for ESP32-S3
 * CRITICAL: Must be initialized BEFORE display
 */

#include "hal.h"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_log.h>
#include <esp_wifi.h>

static const char* TAG = "WIFI_HAL";

// WiFi state
static WiFiStatus_t wifi_status = WIFI_STATUS_DISCONNECTED;
static bool wifi_initialized = false;

/**
 * @brief Initialize WiFi subsystem
 * CRITICAL: Called BEFORE display_hal_init()
 */
int wifi_hal_init(void) {
    ESP_LOGI(TAG, "Initializing WiFi (BEFORE display)...");
    
    // Set WiFi mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    wifi_initialized = true;
    wifi_status = WIFI_STATUS_DISCONNECTED;
    
    ESP_LOGI(TAG, "WiFi HAL initialized successfully");
    return 0;
}

/**
 * @brief Connect to WiFi network
 */
int wifi_hal_connect(const WiFiConfig_t* config) {
    if (!wifi_initialized) {
        ESP_LOGE(TAG, "WiFi not initialized");
        return -1;
    }
    
    if (!config || !config->ssid) {
        ESP_LOGE(TAG, "Invalid WiFi configuration");
        return -2;
    }
    
    ESP_LOGI(TAG, "Connecting to WiFi: %s", config->ssid);
    wifi_status = WIFI_STATUS_CONNECTING;
    
    // Start connection
    WiFi.begin(config->ssid, config->password);
    
    // Wait for connection with timeout
    uint32_t start_time = millis();
    uint32_t timeout = config->timeout_ms > 0 ? config->timeout_ms : 10000;
    
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start_time > timeout) {
            ESP_LOGW(TAG, "WiFi connection timeout");
            wifi_status = WIFI_STATUS_ERROR;
            return -3;
        }
        delay(100);
    }
    
    wifi_status = WIFI_STATUS_CONNECTED;
    ESP_LOGI(TAG, "WiFi connected! IP: %s", WiFi.localIP().toString().c_str());
    ESP_LOGI(TAG, "Signal strength: %d dBm", WiFi.RSSI());
    
    return 0;
}

/**
 * @brief Disconnect from WiFi
 */
void wifi_hal_disconnect(void) {
    if (!wifi_initialized) return;
    
    ESP_LOGI(TAG, "Disconnecting WiFi");
    WiFi.disconnect();
    wifi_status = WIFI_STATUS_DISCONNECTED;
}

/**
 * @brief Get WiFi status
 */
WiFiStatus_t wifi_hal_get_status(void) {
    if (!wifi_initialized) {
        return WIFI_STATUS_DISCONNECTED;
    }
    
    // Update status based on WiFi state
    wl_status_t wl_status = WiFi.status();
    
    switch (wl_status) {
        case WL_CONNECTED:
            wifi_status = WIFI_STATUS_CONNECTED;
            break;
        case WL_DISCONNECTED:
            wifi_status = WIFI_STATUS_DISCONNECTED;
            break;
        case WL_CONNECT_FAILED:
            wifi_status = WIFI_STATUS_ERROR;
            break;
        default:
            wifi_status = WIFI_STATUS_CONNECTING;
            break;
    }
    
    return wifi_status;
}

/**
 * @brief Get WiFi signal strength
 */
int8_t wifi_hal_get_rssi(void) {
    if (!wifi_initialized || wifi_status != WIFI_STATUS_CONNECTED) {
        return 0;
    }
    
    return WiFi.RSSI();
}

/**
 * @brief Get local IP address
 */
int wifi_hal_get_ip(char* ip_str) {
    if (!wifi_initialized || !ip_str) {
        return -1;
    }
    
    if (wifi_status != WIFI_STATUS_CONNECTED) {
        strcpy(ip_str, "0.0.0.0");
        return -2;
    }
    
    strcpy(ip_str, WiFi.localIP().toString().c_str());
    return 0;
}
