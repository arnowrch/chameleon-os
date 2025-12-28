/**
 * @file event_bus.cpp
 * @brief Event Bus implementation
 */

#include "event_bus.h"
#include <Arduino.h>
#include <esp_log.h>
#include <string.h>

static const char* TAG = "EVENT_BUS";

// Subscriber list for each event type
typedef struct {
    EventHandler_t handlers[MAX_SUBSCRIBERS_PER_EVENT];
    uint8_t count;
} SubscriberList_t;

// Event bus state
static SubscriberList_t subscribers[MAX_EVENT_TYPES];
static bool event_bus_initialized = false;

/**
 * @brief Initialize event bus
 */
int event_bus_init(void) {
    ESP_LOGI(TAG, "Initializing event bus...");
    
    // Clear all subscribers
    memset(subscribers, 0, sizeof(subscribers));
    
    event_bus_initialized = true;
    ESP_LOGI(TAG, "Event bus initialized successfully");
    return 0;
}

/**
 * @brief Subscribe to an event
 */
int event_bus_subscribe(EventType_t type, EventHandler_t handler) {
    if (!event_bus_initialized) {
        ESP_LOGE(TAG, "Event bus not initialized");
        return -1;
    }
    
    if (type >= MAX_EVENT_TYPES) {
        ESP_LOGE(TAG, "Invalid event type: %d", type);
        return -2;
    }
    
    if (!handler) {
        ESP_LOGE(TAG, "Invalid handler");
        return -3;
    }
    
    SubscriberList_t* list = &subscribers[type];
    
    // Check if already subscribed
    for (int i = 0; i < list->count; i++) {
        if (list->handlers[i] == handler) {
            ESP_LOGW(TAG, "Handler already subscribed to event %d", type);
            return 0;
        }
    }
    
    // Check if list is full
    if (list->count >= MAX_SUBSCRIBERS_PER_EVENT) {
        ESP_LOGE(TAG, "Subscriber list full for event %d", type);
        return -4;
    }
    
    // Add handler
    list->handlers[list->count++] = handler;
    ESP_LOGD(TAG, "Subscribed to event %d (total: %d)", type, list->count);
    
    return 0;
}

/**
 * @brief Unsubscribe from an event
 */
int event_bus_unsubscribe(EventType_t type, EventHandler_t handler) {
    if (!event_bus_initialized) {
        ESP_LOGE(TAG, "Event bus not initialized");
        return -1;
    }
    
    if (type >= MAX_EVENT_TYPES) {
        ESP_LOGE(TAG, "Invalid event type: %d", type);
        return -2;
    }
    
    SubscriberList_t* list = &subscribers[type];
    
    // Find and remove handler
    for (int i = 0; i < list->count; i++) {
        if (list->handlers[i] == handler) {
            // Shift remaining handlers
            for (int j = i; j < list->count - 1; j++) {
                list->handlers[j] = list->handlers[j + 1];
            }
            list->count--;
            ESP_LOGD(TAG, "Unsubscribed from event %d", type);
            return 0;
        }
    }
    
    ESP_LOGW(TAG, "Handler not found for event %d", type);
    return -3;
}

/**
 * @brief Publish an event
 */
int event_bus_publish(Event_t* event) {
    if (!event_bus_initialized) {
        ESP_LOGE(TAG, "Event bus not initialized");
        return -1;
    }
    
    if (!event) {
        ESP_LOGE(TAG, "Invalid event");
        return -2;
    }
    
    if (event->type >= MAX_EVENT_TYPES) {
        ESP_LOGE(TAG, "Invalid event type: %d", event->type);
        return -3;
    }
    
    // Set timestamp
    event->timestamp = millis();
    
    // Get subscriber list
    SubscriberList_t* list = &subscribers[event->type];
    
    // Call all handlers
    for (int i = 0; i < list->count; i++) {
        if (list->handlers[i]) {
            list->handlers[i](event);
        }
    }
    
    return 0;
}

/**
 * @brief Process pending events
 */
void event_bus_process(void) {
    // Currently all events are processed immediately in publish()
    // This function reserved for future queue-based implementation
}
