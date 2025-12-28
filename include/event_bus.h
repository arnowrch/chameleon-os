/**
 * @file event_bus.h
 * @brief Event Bus system for Chameleon OS
 * 
 * Publish-subscribe event system for decoupled component communication
 */

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <stdint.h>
#include <stdbool.h>

// Event types
typedef enum {
    EVENT_NONE = 0,
    EVENT_TOUCH,
    EVENT_WIFI_STATUS,
    EVENT_APP_SWITCH,
    EVENT_THEME_CHANGE,
    EVENT_TIME_SYNC,
    EVENT_BUTTON_PRESS,
    EVENT_CUSTOM_START = 100  // Custom events start here
} EventType_t;

// Event structure
typedef struct {
    EventType_t type;
    void* data;
    uint32_t size;
    uint32_t timestamp;
} Event_t;

// Event handler callback
typedef void (*EventHandler_t)(Event_t* event);

// Maximum number of subscribers per event type
#define MAX_SUBSCRIBERS_PER_EVENT 8
#define MAX_EVENT_TYPES 32

/**
 * @brief Initialize event bus
 * @return 0 on success, negative on error
 */
int event_bus_init(void);

/**
 * @brief Subscribe to an event type
 * @param type Event type to subscribe to
 * @param handler Callback function to handle events
 * @return 0 on success, negative on error
 */
int event_bus_subscribe(EventType_t type, EventHandler_t handler);

/**
 * @brief Unsubscribe from an event type
 * @param type Event type to unsubscribe from
 * @param handler Handler function to remove
 * @return 0 on success, negative on error
 */
int event_bus_unsubscribe(EventType_t type, EventHandler_t handler);

/**
 * @brief Publish an event
 * @param event Event to publish
 * @return 0 on success, negative on error
 */
int event_bus_publish(Event_t* event);

/**
 * @brief Process pending events
 * Should be called in main loop
 */
void event_bus_process(void);

#endif // EVENT_BUS_H
