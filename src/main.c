/**
 * @file main.c
 * @brief Chameleon OS - Main Entry Point
 * 
 * Production-ready embedded OS for ESP32-S3 with touch display
 * Transforms into 33 specialized applications from a single hardware platform
 * 
 * @author Chameleon OS Team
 * @date 2025-12-29
 */

#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "nvs_flash.h"
#include "esp_lvgl_port.h"

// BSP Hardware Abstraction Layer
#include "bsp_display.h"
#include "bsp_touch.h"
#include "bsp_i2c.h"
#include "bsp_wifi.h"
#include "bsp_battery.h"
#include "bsp_qmi8658.h"
#include "bsp_sdcard.h"

// Configuration
#define CHAMELEON_DISPLAY_ROTATION 90

#if CHAMELEON_DISPLAY_ROTATION == 90 || CHAMELEON_DISPLAY_ROTATION == 270
#define CHAMELEON_LCD_H_RES (320)
#define CHAMELEON_LCD_V_RES (172)
#else
#define CHAMELEON_LCD_H_RES (172)
#define CHAMELEON_LCD_V_RES (320)
#endif

#define CHAMELEON_LCD_DRAW_BUFF_HEIGHT (50)  // Optimal for DMA performance
#define CHAMELEON_LCD_DRAW_BUFF_DOUBLE (1)   // Double buffering for 60 FPS

static const char *TAG = "chameleon_os";

/* Hardware Handles */
static esp_lcd_panel_io_handle_t io_handle = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;

/* LVGL Handles */
static lv_display_t *lvgl_disp = NULL;
static lv_indev_t *lvgl_touch_indev = NULL;

/**
 * @brief Initialize LVGL port
 * @return ESP_OK on success
 */
static esp_err_t chameleon_lvgl_init(void)
{
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,       /* LVGL task priority */
        .task_stack = 1024 * 10,  /* LVGL task stack size */
        .task_affinity = -1,      /* No core affinity */
        .task_max_sleep_ms = 500, /* Maximum sleep in LVGL task */
        .timer_period_ms = 5      /* 5ms tick for smooth animations */
    };
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port init failed");

    /* Add LCD screen */
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = CHAMELEON_LCD_H_RES * CHAMELEON_LCD_DRAW_BUFF_HEIGHT,
        .double_buffer = CHAMELEON_LCD_DRAW_BUFF_DOUBLE,
        .hres = CHAMELEON_LCD_H_RES,
        .vres = CHAMELEON_LCD_V_RES,
        .monochrome = false,
        .rotation = {
            .swap_xy = (CHAMELEON_DISPLAY_ROTATION == 90 || CHAMELEON_DISPLAY_ROTATION == 270),
            .mirror_x = (CHAMELEON_DISPLAY_ROTATION == 90 || CHAMELEON_DISPLAY_ROTATION == 180),
            .mirror_y = (CHAMELEON_DISPLAY_ROTATION == 0 || CHAMELEON_DISPLAY_ROTATION == 90),
        }
    };
    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    /* Add touch input */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };
    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

    ESP_LOGI(TAG, "LVGL initialized successfully");
    return ESP_OK;
}

/**
 * @brief Chameleon OS Main Entry Point
 * 
 * Golden Path Initialization Sequence (CRITICAL - DO NOT CHANGE ORDER):
 * 1. NVS Flash
 * 2. I2C Bus
 * 3. IMU (QMI8658) - Non-fatal
 * 4. Battery Monitor - Non-fatal
 * 5. WiFi (MUST be before Display!)
 * 6. Display (JD9853 with Panel Gap 0,34)
 * 7. Touch (AXS5106)
 * 8. SD Card - Non-fatal
 * 9. LVGL Port
 * 10. Backlight
 * 11. UI Initialization
 */
void app_main(void)
{
    ESP_LOGI(TAG, "🦎 Chameleon OS - Starting...");
    ESP_LOGI(TAG, "Version: Phase 1 - Infrastructure");
    ESP_LOGI(TAG, "Hardware: ESP32-S3 with JD9853 Display (320x172)");

    // ═══════════════════════════════════════════════════════════════
    // STEP 1: NVS Flash Initialization
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[1/10] Initializing NVS Flash...");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition truncated, erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "✓ NVS Flash ready");

    // ═══════════════════════════════════════════════════════════════
    // STEP 2: I2C Bus Initialization
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[2/10] Initializing I2C Bus...");
    i2c_master_bus_handle_t i2c_bus_handle = bsp_i2c_init();
    if (i2c_bus_handle == NULL) {
        ESP_LOGE(TAG, "Failed to initialize I2C bus");
        return;
    }
    ESP_LOGI(TAG, "✓ I2C Bus ready");

    // ═══════════════════════════════════════════════════════════════
    // STEP 3: IMU Initialization (Non-Fatal)
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[3/10] Initializing IMU (QMI8658)...");
    esp_err_t imu_ret = bsp_qmi8658_init(i2c_bus_handle);
    if (imu_ret != ESP_OK) {
        ESP_LOGW(TAG, "⚠ QMI8658 IMU init failed, continuing without IMU");
    } else {
        ESP_LOGI(TAG, "✓ IMU ready");
    }

    // ═══════════════════════════════════════════════════════════════
    // STEP 4: Battery Monitor Initialization (Non-Fatal)
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[4/10] Initializing Battery Monitor...");
    esp_err_t battery_ret = bsp_battery_init();
    if (battery_ret != ESP_OK) {
        ESP_LOGW(TAG, "⚠ Battery monitor init failed, continuing without battery");
    } else {
        ESP_LOGI(TAG, "✓ Battery monitor ready");
    }

    // ═══════════════════════════════════════════════════════════════
    // STEP 5: WiFi Initialization (MUST BE BEFORE DISPLAY!)
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[5/10] Initializing WiFi...");
    ESP_LOGI(TAG, "⚠ CRITICAL: WiFi MUST initialize before Display!");
    // TODO: Get WiFi credentials from NVS or config
    bsp_wifi_init("YourSSID", "YourPassword");  // Replace with actual credentials
    ESP_LOGI(TAG, "✓ WiFi initialized");

    // ═══════════════════════════════════════════════════════════════
    // STEP 6: Display Initialization (Panel Gap 0,34 at 90°)
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[6/10] Initializing Display (JD9853)...");
    ESP_LOGI(TAG, "⚠ Panel Gap: (0, 34) at 90° rotation");
    bsp_display_init(&io_handle, &panel_handle, CHAMELEON_LCD_H_RES * CHAMELEON_LCD_DRAW_BUFF_HEIGHT);
    ESP_LOGI(TAG, "✓ Display ready (320x172 @ 90°)");

    // ═══════════════════════════════════════════════════════════════
    // STEP 7: Touch Initialization (AXS5106)
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[7/10] Initializing Touch Controller (AXS5106)...");
    bsp_touch_init(&touch_handle, i2c_bus_handle, CHAMELEON_LCD_H_RES, CHAMELEON_LCD_V_RES, CHAMELEON_DISPLAY_ROTATION);
    ESP_LOGI(TAG, "✓ Touch ready");

    // ═══════════════════════════════════════════════════════════════
    // STEP 8: SD Card Initialization (Non-Fatal)
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[8/10] Initializing SD Card...");
    esp_err_t sd_ret = bsp_sdcard_init();
    if (sd_ret != ESP_OK) {
        ESP_LOGW(TAG, "⚠ SD Card init failed, continuing without SD");
    } else {
        ESP_LOGI(TAG, "✓ SD Card ready");
    }

    // ═══════════════════════════════════════════════════════════════
    // STEP 9: LVGL Port Initialization
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[9/10] Initializing LVGL...");
    ESP_ERROR_CHECK(chameleon_lvgl_init());
    ESP_LOGI(TAG, "✓ LVGL ready");

    // ═══════════════════════════════════════════════════════════════
    // STEP 10: Backlight Control
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "[10/10] Enabling Backlight...");
    bsp_display_brightness_init();
    bsp_display_set_brightness(100);  // Full brightness
    ESP_LOGI(TAG, "✓ Backlight at 100%%");

    // ═══════════════════════════════════════════════════════════════
    // UI Initialization (Simple test screen for Phase 1)
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "Creating test UI...");
    if (lvgl_port_lock(0)) {
        lv_obj_t *scr = lv_scr_act();
        lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);  // Black background

        // Title label
        lv_obj_t *label = lv_label_create(scr);
        lv_label_set_text(label, "🦎 Chameleon OS\nPhase 1: Infrastructure\n\nSystem Ready!");
        lv_obj_set_style_text_color(label, lv_color_hex(0x00FF00), 0);  // Green text
        lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

        lvgl_port_unlock();
    }

    // ═══════════════════════════════════════════════════════════════
    // System Ready
    // ═══════════════════════════════════════════════════════════════
    ESP_LOGI(TAG, "═══════════════════════════════════════════════════");
    ESP_LOGI(TAG, "🚀 Chameleon OS - System Ready!");
    ESP_LOGI(TAG, "Heap Free: %ld bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "═══════════════════════════════════════════════════");
}
