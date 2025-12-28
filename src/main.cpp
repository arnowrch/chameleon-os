/**
 * @file main.cpp
 * @brief Chameleon OS - Main entry point
 * 
 * ESP32-S3 Universal Embedded OS
 * 320x172 Touch Display, 33 Apps, 60 FPS Luxury UX
 * 
 * CRITICAL: Golden Path Initialization
 * WiFi MUST initialize BEFORE Display
 */

#include <Arduino.h>
#include <esp_log.h>
#include <esp_timer.h>

// HAL
#include "hal.h"

// Core Systems
#include "event_bus.h"
#include "theme_manager.h"
#include "ui_framework.h"

// App Framework
#include "app_framework.h"

// Apps
extern App_t app_home;
extern App_t app_clock;
extern App_t app_wifi;

static const char* TAG = "MAIN";

// Performance tracking
static int64_t frame_start_time = 0;
static const int64_t TARGET_FRAME_TIME_US = 16667;  // 60 FPS = 16.67ms

// WiFi configuration (example - would come from settings)
static WiFiConfig_t wifi_config = {
    .ssid = "YourSSID",
    .password = "YourPassword",
    .timeout_ms = 10000
};

/**
 * @brief Golden Path Initialization
 * CRITICAL: WiFi before Display!
 */
void setup() {
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "   Chameleon OS - Universal Embedded OS");
    ESP_LOGI(TAG, "   ESP32-S3 + 320x172 Touch Display");
    ESP_LOGI(TAG, "   Target: 60 FPS Luxury UX");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "");
    
    // 1. System Init (Serial for logging)
    Serial.begin(115200);
    delay(100);
    ESP_LOGI(TAG, "Step 1: System initialized");
    
    // 2. WiFi Init (BEFORE Display - CRITICAL!)
    ESP_LOGI(TAG, "Step 2: Initializing WiFi (BEFORE display)...");
    if (wifi_hal_init() != 0) {
        ESP_LOGE(TAG, "WiFi initialization failed");
    }
    
    // Optional: Connect to WiFi (non-blocking approach)
    // Uncomment and set credentials to actually connect
    // wifi_hal_connect(&wifi_config);
    ESP_LOGI(TAG, "WiFi HAL ready (connection optional)");
    
    // 3. Event Bus Init
    ESP_LOGI(TAG, "Step 3: Initializing Event Bus...");
    if (event_bus_init() != 0) {
        ESP_LOGE(TAG, "Event Bus initialization failed");
        return;
    }
    
    // 4. Display Init (with double-buffer, after WiFi)
    ESP_LOGI(TAG, "Step 4: Initializing Display (320x172, double-buffer)...");
    if (display_hal_init() != 0) {
        ESP_LOGE(TAG, "Display initialization failed");
        return;
    }
    ESP_LOGI(TAG, "Display ready: Panel Gap (0, 34), Effective area: 320x138");
    
    // 5. Touch Init
    ESP_LOGI(TAG, "Step 5: Initializing Touch...");
    if (touch_hal_init() != 0) {
        ESP_LOGE(TAG, "Touch initialization failed");
    }
    
    // 6. Theme Manager Init
    ESP_LOGI(TAG, "Step 6: Initializing Theme Manager...");
    if (theme_manager_init() != 0) {
        ESP_LOGE(TAG, "Theme Manager initialization failed");
        return;
    }
    
    // 7. NTP Init (time sync)
    ESP_LOGI(TAG, "Step 7: Initializing NTP...");
    if (ntp_hal_init() != 0) {
        ESP_LOGE(TAG, "NTP initialization failed");
    }
    
    // Optional: Sync time (requires WiFi connection)
    // ntp_hal_sync("pool.ntp.org", 5000);
    
    // 8. App Framework Init
    ESP_LOGI(TAG, "Step 8: Initializing App Framework...");
    if (app_framework_init() != 0) {
        ESP_LOGE(TAG, "App Framework initialization failed");
        return;
    }
    
    // 9. Load Apps (registering sample apps)
    ESP_LOGI(TAG, "Step 9: Loading apps...");
    app_register(&app_home);
    app_register(&app_clock);
    app_register(&app_wifi);
    
    ESP_LOGI(TAG, "Loaded %d apps (targeting 33 total)", app_get_count());
    
    // 10. Focus-Carousel UI Init
    ESP_LOGI(TAG, "Step 10: Initializing Focus-Carousel UI...");
    if (ui_framework_init() != 0) {
        ESP_LOGE(TAG, "UI Framework initialization failed");
        return;
    }
    
    // 11. Enter first app (Home)
    ESP_LOGI(TAG, "Step 11: Launching default app...");
    app_switch(0);
    
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "   Chameleon OS Ready!");
    ESP_LOGI(TAG, "   Entering Main Loop (60 FPS target)");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "");
    
    frame_start_time = esp_timer_get_time();
}

/**
 * @brief Main Loop - 60 FPS
 * Frame budget: 16.67ms
 * - Input: 2ms
 * - Update: 5ms
 * - Render: 8ms
 * - Swap: 1.67ms
 */
void loop() {
    // Start frame timing
    int64_t loop_start = esp_timer_get_time();
    
    // 1. Input Processing (~2ms budget)
    TouchEvent_t touch;
    if (touch_hal_read(&touch)) {
        ui_process_touch(touch.x, touch.y, touch.pressed);
        
        // Publish touch event
        Event_t event = {
            .type = EVENT_TOUCH,
            .data = &touch,
            .size = sizeof(touch),
            .timestamp = 0
        };
        event_bus_publish(&event);
    }
    
    // 2. App Update (~5ms budget)
    app_update();
    
    // 3. Render (~8ms budget)
    ui_render();
    
    // 4. Event Bus Processing
    event_bus_process();
    
    // Frame timing and FPS control
    int64_t frame_time = esp_timer_get_time() - loop_start;
    
    // Log if frame took too long
    if (frame_time > TARGET_FRAME_TIME_US) {
        ESP_LOGW(TAG, "Frame time exceeded: %lld us (target: %lld us)", 
                 frame_time, TARGET_FRAME_TIME_US);
    }
    
    // Sleep to maintain 60 FPS
    int64_t sleep_time = TARGET_FRAME_TIME_US - frame_time;
    if (sleep_time > 0) {
        delayMicroseconds(sleep_time);
    }
    
    // Update frame start time
    frame_start_time = esp_timer_get_time();
}
