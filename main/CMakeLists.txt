# CMakeLists.txt for the main component

# The main component should depend on the components in the 'components' directory
# and the LVGL components.

idf_component_register(SRCS "main.c" "lvgl_ui.c" "ui_router.c" "ui_theme.c" "ui_components.c" "ui_fonts.c"
                    INCLUDE_DIRS "."
                    REQUIRES "esp_lcd" "esp_lcd_touch" "nvs_flash" "esp_lvgl_port" "lvgl" "bsp" "driver" "iot_button")
