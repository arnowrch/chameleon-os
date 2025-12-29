idf_component_register(SRCS "bsp_i2c.c" "bsp_wifi.c"
                    INCLUDE_DIRS "."
                    REQUIRES "esp_wifi" "nvs_flash" "esp_netif" "driver")
