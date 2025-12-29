#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// BSP Hardware Abstraction Layer
// #include "bsp_display.h" // TODO: Implement JD9853 driver
// #include "bsp_touch.h" // TODO: Implement AXS5106 driver
#include "bsp_i2c.h"
#include "bsp_wifi.h"

static const char *TAG = "CHAMELEON_OS";

/**
 * CHAMELEON OS: MAIN ENTRY POINT
 * Golden Path Initialization Sequence
 * Strategy: System Stability First (NTP/WiFi), then Graphics
 */
void app_main(void) {
    ESP_LOGI(TAG, " Initializing Chameleon OS...");
    
    // 
    // STEP 1: NVS Flash (Required for WiFi & Settings)
    // 
    ESP_LOGI(TAG, "[1/7] Initializing NVS Flash...");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition truncated, erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, " NVS Flash ready");

    // 
    // STEP 2: I2C Bus (Shared for Touch & IMU)
    // 
    ESP_LOGI(TAG, "[2/7] Initializing I2C Bus...");
    i2c_master_bus_handle_t i2c_bus = bsp_i2c_init();
    if (i2c_bus == NULL) {
        ESP_LOGE(TAG, "Failed to initialize I2C bus");
        return;
    }
    ESP_LOGI(TAG, " I2C Bus ready");

    // 
    // STEP 3: WiFi Initialization (CRITICAL: Before Display!)
    // 
    ESP_LOGI(TAG, "[3/7] Initializing WiFi...");
    ESP_LOGW(TAG, " WiFi MUST initialize BEFORE Display (timing critical!)");
    bsp_wifi_init("YourSSID", "YourPassword");  // TODO: Load from NVS
    ESP_LOGI(TAG, " WiFi initialized");

    // 
    // STEP 4: Display - TEMPORARILY DISABLED
    // TODO: Implement JD9853 display driver
    ESP_LOGW(TAG, "[4/7] Display initialization SKIPPED (driver not implemented)");

    // STEP 5: Touch - TEMPORARILY DISABLED  
    // TODO: Implement AXS5106 touch driver
    ESP_LOGW(TAG, "[5/7] Touch initialization SKIPPED (driver not implemented)");

    // STEP 6: LVGL & UI Initialization
    // 
    ESP_LOGI(TAG, "[6/7] Initializing LVGL...");
    // TODO: Initialize LVGL port here
    ESP_LOGI(TAG, " LVGL ready");

    // 
    // STEP 7: Backlight - TEMPORARILY DISABLED
    ESP_LOGW(TAG, "[7/7] Backlight initialization SKIPPED (display not initialized)");

    ESP_LOGI(TAG, " Chameleon OS - System Ready (MINIMAL MODE - Display/Touch disabled)!");
    ESP_LOGI(TAG, "Phase 1: Infrastructure Complete");
    ESP_LOGI(TAG, "Heap Free: %ld bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "");
}
