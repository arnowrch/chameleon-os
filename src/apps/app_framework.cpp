/**
 * @file app_framework.cpp
 * @brief App framework implementation
 */

#include "app_framework.h"
#include "event_bus.h"
#include <Arduino.h>
#include <esp_log.h>

static const char* TAG = "APP_FW";

// App registry
static App_t* apps[MAX_APPS];
static int app_count = 0;
static int current_app_id = -1;
static bool framework_initialized = false;

/**
 * @brief Initialize app framework
 */
int app_framework_init(void) {
    ESP_LOGI(TAG, "Initializing app framework...");
    
    // Clear app registry
    for (int i = 0; i < MAX_APPS; i++) {
        apps[i] = nullptr;
    }
    
    app_count = 0;
    current_app_id = -1;
    
    framework_initialized = true;
    ESP_LOGI(TAG, "App framework initialized");
    return 0;
}

/**
 * @brief Register an app
 */
int app_register(App_t* app) {
    if (!framework_initialized) {
        ESP_LOGE(TAG, "Framework not initialized");
        return -1;
    }
    
    if (!app) {
        ESP_LOGE(TAG, "Invalid app");
        return -2;
    }
    
    if (app_count >= MAX_APPS) {
        ESP_LOGE(TAG, "App registry full");
        return -3;
    }
    
    // Assign app ID
    int app_id = app_count;
    apps[app_id] = app;
    app_count++;
    
    // Initialize app
    if (app->init) {
        app->init();
    }
    
    ESP_LOGI(TAG, "Registered app #%d: %s (sector %d)", 
             app_id, app->name, app->sector);
    
    return app_id;
}

/**
 * @brief Get app count
 */
int app_get_count(void) {
    return app_count;
}

/**
 * @brief Get app by ID
 */
App_t* app_get(int app_id) {
    if (app_id < 0 || app_id >= app_count) {
        return nullptr;
    }
    return apps[app_id];
}

/**
 * @brief Switch to app
 */
int app_switch(int app_id) {
    if (!framework_initialized) {
        ESP_LOGE(TAG, "Framework not initialized");
        return -1;
    }
    
    if (app_id < 0 || app_id >= app_count) {
        ESP_LOGE(TAG, "Invalid app ID: %d", app_id);
        return -2;
    }
    
    // Exit current app
    if (current_app_id >= 0 && apps[current_app_id]) {
        App_t* current = apps[current_app_id];
        if (current->on_exit) {
            current->on_exit();
        }
    }
    
    // Switch to new app
    current_app_id = app_id;
    App_t* new_app = apps[app_id];
    
    ESP_LOGI(TAG, "Switching to app #%d: %s", app_id, new_app->name);
    
    // Enter new app
    if (new_app->on_enter) {
        new_app->on_enter();
    }
    
    // Publish app switch event
    Event_t event = {
        .type = EVENT_APP_SWITCH,
        .data = &app_id,
        .size = sizeof(app_id),
        .timestamp = 0
    };
    event_bus_publish(&event);
    
    return 0;
}

/**
 * @brief Get current app ID
 */
int app_get_current(void) {
    return current_app_id;
}

/**
 * @brief Update current app
 */
void app_update(void) {
    if (current_app_id >= 0 && apps[current_app_id]) {
        App_t* app = apps[current_app_id];
        if (app->on_update) {
            app->on_update();
        }
    }
}
