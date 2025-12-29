# CMakeLists.txt for the bsp component

idf_component_register(SRCS "bsp_display.c" "bsp_touch.c" "bsp_i2c.c" "bsp_wifi.c" "bsp_sdcard.c" "bsp_battery.c"
                    INCLUDE_DIRS "."
                    REQUIRES "esp_lcd" "esp_lcd_touch" "esp_lcd_gc9a01" "esp_lcd_st7789" "driver" "esp_wifi" "nvs_flash" "esp_netif" "fatfs" "sdmmc" "esp_adc" "esp_pm")
