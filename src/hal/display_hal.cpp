/**
 * @file display_hal.c
 * @brief Display HAL implementation for Waveshare 320x172 with double-buffer
 */

#include "hal.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp_log.h>
#include <esp_heap_caps.h>

static const char* TAG = "DISPLAY_HAL";

// TFT_eSPI instance
static TFT_eSPI* tft = nullptr;

// Double buffer (in PSRAM for large framebuffers)
static uint16_t* back_buffer = nullptr;
static bool double_buffer_enabled = true;

// Frame counter for FPS tracking
static uint32_t frame_count = 0;

// Brightness control
static uint8_t current_brightness = 255;

/**
 * @brief Initialize display with double buffering
 */
int display_hal_init(void) {
    ESP_LOGI(TAG, "Initializing display (320x172, double-buffer)...");
    
    // Create TFT instance
    tft = new TFT_eSPI();
    if (!tft) {
        ESP_LOGE(TAG, "Failed to create TFT instance");
        return -1;
    }
    
    // Initialize TFT
    tft->init();
    tft->setRotation(1);  // Landscape mode
    tft->fillScreen(TFT_BLACK);
    
    ESP_LOGI(TAG, "Display initialized: %dx%d", DISPLAY_WIDTH, DISPLAY_HEIGHT);
    ESP_LOGI(TAG, "Panel gap: top=%d, bottom=%d", PANEL_GAP_TOP, PANEL_GAP_BOTTOM);
    ESP_LOGI(TAG, "Effective area: %dx%d", DISPLAY_WIDTH, EFFECTIVE_HEIGHT);
    
    // Allocate back buffer in PSRAM for double buffering
    size_t buffer_size = DISPLAY_WIDTH * EFFECTIVE_HEIGHT * sizeof(uint16_t);
    back_buffer = (uint16_t*)heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    
    if (!back_buffer) {
        ESP_LOGW(TAG, "PSRAM allocation failed, trying regular heap");
        back_buffer = (uint16_t*)malloc(buffer_size);
    }
    
    if (!back_buffer) {
        ESP_LOGE(TAG, "Failed to allocate back buffer (%d bytes)", buffer_size);
        double_buffer_enabled = false;
        return -2;
    }
    
    ESP_LOGI(TAG, "Back buffer allocated: %d bytes in %s", 
             buffer_size, 
             heap_caps_get_allocated_size(back_buffer) ? "PSRAM" : "SRAM");
    
    // Clear back buffer
    memset(back_buffer, 0, buffer_size);
    
    // Set initial brightness
    display_hal_set_brightness(current_brightness);
    
    ESP_LOGI(TAG, "Display HAL initialized successfully");
    return 0;
}

/**
 * @brief Clear back buffer with color
 */
void display_hal_clear(uint16_t color) {
    if (!double_buffer_enabled || !back_buffer) {
        if (tft) tft->fillScreen(color);
        return;
    }
    
    // Fill back buffer with color
    for (int i = 0; i < DISPLAY_WIDTH * EFFECTIVE_HEIGHT; i++) {
        back_buffer[i] = color;
    }
}

/**
 * @brief Draw pixel to back buffer
 */
void display_hal_pixel(int x, int y, uint16_t color) {
    // Bounds checking
    if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= EFFECTIVE_HEIGHT) {
        return;
    }
    
    if (!double_buffer_enabled || !back_buffer) {
        if (tft) tft->drawPixel(x, y + PANEL_GAP_TOP, color);
        return;
    }
    
    // Write to back buffer
    back_buffer[y * DISPLAY_WIDTH + x] = color;
}

/**
 * @brief Draw filled rectangle
 */
void display_hal_rect(int x, int y, int w, int h, uint16_t color) {
    // Bounds checking and clipping
    if (x >= DISPLAY_WIDTH || y >= EFFECTIVE_HEIGHT) return;
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > DISPLAY_WIDTH) w = DISPLAY_WIDTH - x;
    if (y + h > EFFECTIVE_HEIGHT) h = EFFECTIVE_HEIGHT - y;
    if (w <= 0 || h <= 0) return;
    
    if (!double_buffer_enabled || !back_buffer) {
        if (tft) tft->fillRect(x, y + PANEL_GAP_TOP, w, h, color);
        return;
    }
    
    // Draw to back buffer
    for (int dy = 0; dy < h; dy++) {
        int offset = (y + dy) * DISPLAY_WIDTH + x;
        for (int dx = 0; dx < w; dx++) {
            back_buffer[offset + dx] = color;
        }
    }
}

/**
 * @brief Draw line using Bresenham's algorithm
 */
void display_hal_line(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        display_hal_pixel(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/**
 * @brief Draw text (simplified, using TFT_eSPI for now)
 */
void display_hal_text(int x, int y, const char* text, uint16_t color, uint8_t size) {
    if (!tft || !text) return;
    
    // Bounds check
    if (x < 0 || y < 0 || y >= EFFECTIVE_HEIGHT) return;
    
    // For double-buffered mode, we need to draw directly to display
    // (text rendering to buffer would require font bitmap implementation)
    tft->setTextColor(color);
    tft->setTextSize(size);
    tft->drawString(text, x, y + PANEL_GAP_TOP);
}

/**
 * @brief Swap buffers (present frame)
 */
void display_hal_swap(void) {
    if (!tft) return;
    
    if (!double_buffer_enabled || !back_buffer) {
        // No double buffering, nothing to swap
        frame_count++;
        return;
    }
    
    // Push back buffer to display using DMA for speed
    tft->setAddrWindow(0, PANEL_GAP_TOP, DISPLAY_WIDTH, EFFECTIVE_HEIGHT);
    tft->pushColors(back_buffer, DISPLAY_WIDTH * EFFECTIVE_HEIGHT);
    
    frame_count++;
}

/**
 * @brief Set display brightness
 */
void display_hal_set_brightness(uint8_t level) {
    current_brightness = level;
    
    // TFT_eSPI doesn't have direct brightness control
    // This would need to be implemented via PWM on backlight pin
    // For now, just store the value
    ESP_LOGD(TAG, "Brightness set to: %d", level);
}

/**
 * @brief Get frame count
 */
uint32_t display_hal_get_frame_count(void) {
    return frame_count;
}
