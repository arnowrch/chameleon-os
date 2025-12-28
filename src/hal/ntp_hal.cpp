/**
 * @file ntp_hal.cpp
 * @brief NTP time synchronization HAL implementation
 */

#include "hal.h"
#include <Arduino.h>
#include <esp_log.h>
#include <esp_sntp.h>
#include <time.h>

static const char* TAG = "NTP_HAL";

// NTP state
static bool ntp_initialized = false;
static bool time_synced = false;
static const char* ntp_server = "pool.ntp.org";

/**
 * @brief Initialize NTP client
 */
int ntp_hal_init(void) {
    ESP_LOGI(TAG, "Initializing NTP client...");
    
    // Configure SNTP
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, ntp_server);
    esp_sntp_init();
    
    ntp_initialized = true;
    ESP_LOGI(TAG, "NTP HAL initialized successfully");
    return 0;
}

/**
 * @brief Synchronize time with NTP server
 */
int ntp_hal_sync(const char* server, uint32_t timeout_ms) {
    if (!ntp_initialized) {
        ESP_LOGE(TAG, "NTP not initialized");
        return -1;
    }
    
    if (server) {
        ntp_server = server;
        esp_sntp_setservername(0, server);
    }
    
    ESP_LOGI(TAG, "Synchronizing time with NTP server: %s", ntp_server);
    
    // Wait for time to be set
    time_t now = 0;
    struct tm timeinfo = {0};
    uint32_t start_time = millis();
    uint32_t timeout = timeout_ms > 0 ? timeout_ms : 10000;
    
    while (timeinfo.tm_year < (2020 - 1900)) {
        if (millis() - start_time > timeout) {
            ESP_LOGW(TAG, "NTP sync timeout");
            return -2;
        }
        
        delay(100);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
    
    time_synced = true;
    
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "Time synchronized: %s", strftime_buf);
    
    return 0;
}

/**
 * @brief Get current Unix timestamp
 */
time_t ntp_hal_get_time(void) {
    time_t now;
    time(&now);
    return now;
}

/**
 * @brief Check if time is synchronized
 */
bool ntp_hal_is_synced(void) {
    return time_synced;
}

/**
 * @brief Set timezone
 */
void ntp_hal_set_timezone(const char* tz) {
    if (!tz) return;
    
    ESP_LOGI(TAG, "Setting timezone: %s", tz);
    setenv("TZ", tz, 1);
    tzset();
}
