/**
 * @file hal.h
 * @brief Hardware Abstraction Layer for Chameleon OS
 * 
 * Unified interface for ESP32-S3 hardware:
 * - Display (320x172 with panel gap, double-buffer)
 * - Touch screen
 * - WiFi
 * - NTP time sync
 */

#ifndef HAL_H
#define HAL_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Display configuration
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 172
#define PANEL_GAP_TOP 0
#define PANEL_GAP_BOTTOM 34
#define EFFECTIVE_HEIGHT (DISPLAY_HEIGHT - PANEL_GAP_TOP - PANEL_GAP_BOTTOM)  // 138

// Color definitions (RGB565)
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_GRAY    0x8410

// Touch event types
typedef struct {
    int x;
    int y;
    bool pressed;
    uint32_t timestamp;
} TouchEvent_t;

// WiFi status
typedef enum {
    WIFI_STATUS_DISCONNECTED = 0,
    WIFI_STATUS_CONNECTING,
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_ERROR
} WiFiStatus_t;

// WiFi configuration
typedef struct {
    const char* ssid;
    const char* password;
    uint32_t timeout_ms;
} WiFiConfig_t;

// =============================================================================
// Display HAL
// =============================================================================

/**
 * @brief Initialize display with double buffering
 * @return 0 on success, negative error code on failure
 */
int display_hal_init(void);

/**
 * @brief Clear the back buffer with specified color
 * @param color RGB565 color value
 */
void display_hal_clear(uint16_t color);

/**
 * @brief Draw a pixel to back buffer
 * @param x X coordinate (0-319)
 * @param y Y coordinate (0-137, respecting panel gap)
 * @param color RGB565 color value
 */
void display_hal_pixel(int x, int y, uint16_t color);

/**
 * @brief Draw a filled rectangle
 * @param x X coordinate
 * @param y Y coordinate
 * @param w Width
 * @param h Height
 * @param color RGB565 color
 */
void display_hal_rect(int x, int y, int w, int h, uint16_t color);

/**
 * @brief Draw a line
 * @param x0 Start X
 * @param y0 Start Y
 * @param x1 End X
 * @param y1 End Y
 * @param color RGB565 color
 */
void display_hal_line(int x0, int y0, int x1, int y1, uint16_t color);

/**
 * @brief Draw text
 * @param x X coordinate
 * @param y Y coordinate
 * @param text Text string
 * @param color RGB565 color
 * @param size Font size (1-5)
 */
void display_hal_text(int x, int y, const char* text, uint16_t color, uint8_t size);

/**
 * @brief Swap front and back buffers (present frame)
 * Should be called once per frame after all drawing
 */
void display_hal_swap(void);

/**
 * @brief Set display brightness
 * @param level Brightness level (0-255)
 */
void display_hal_set_brightness(uint8_t level);

/**
 * @brief Get current frame count
 * @return Total frames rendered
 */
uint32_t display_hal_get_frame_count(void);

// =============================================================================
// Touch HAL
// =============================================================================

/**
 * @brief Initialize touch screen
 * @return 0 on success, negative error code on failure
 */
int touch_hal_init(void);

/**
 * @brief Read current touch state
 * @param event Pointer to TouchEvent_t structure to fill
 * @return true if touch event available, false otherwise
 */
bool touch_hal_read(TouchEvent_t* event);

/**
 * @brief Calibrate touch screen
 * @return 0 on success, negative error code on failure
 */
int touch_hal_calibrate(void);

// =============================================================================
// WiFi HAL
// =============================================================================

/**
 * @brief Initialize WiFi subsystem
 * CRITICAL: Must be called BEFORE display_hal_init()
 * @return 0 on success, negative error code on failure
 */
int wifi_hal_init(void);

/**
 * @brief Connect to WiFi network
 * @param config WiFi configuration structure
 * @return 0 on success, negative error code on failure
 */
int wifi_hal_connect(const WiFiConfig_t* config);

/**
 * @brief Disconnect from WiFi network
 */
void wifi_hal_disconnect(void);

/**
 * @brief Get current WiFi status
 * @return WiFiStatus_t enum value
 */
WiFiStatus_t wifi_hal_get_status(void);

/**
 * @brief Get WiFi signal strength
 * @return RSSI in dBm, or 0 if not connected
 */
int8_t wifi_hal_get_rssi(void);

/**
 * @brief Get local IP address
 * @param ip_str Buffer to store IP string (min 16 bytes)
 * @return 0 on success, negative if not connected
 */
int wifi_hal_get_ip(char* ip_str);

// =============================================================================
// NTP HAL
// =============================================================================

/**
 * @brief Initialize NTP client
 * @return 0 on success, negative error code on failure
 */
int ntp_hal_init(void);

/**
 * @brief Synchronize time with NTP server
 * @param server NTP server address (e.g., "pool.ntp.org")
 * @param timeout_ms Timeout in milliseconds
 * @return 0 on success, negative error code on failure
 */
int ntp_hal_sync(const char* server, uint32_t timeout_ms);

/**
 * @brief Get current Unix timestamp
 * @return Unix timestamp (seconds since epoch)
 */
time_t ntp_hal_get_time(void);

/**
 * @brief Check if time is synchronized
 * @return true if synchronized, false otherwise
 */
bool ntp_hal_is_synced(void);

/**
 * @brief Set timezone
 * @param tz Timezone string (e.g., "PST8PDT,M3.2.0,M11.1.0")
 */
void ntp_hal_set_timezone(const char* tz);

#endif // HAL_H
