/**
 * @file theme_manager.h
 * @brief Theme Manager for Chameleon OS
 * 
 * Dynamic theming system for luxury UX
 */

#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <stdint.h>

// Theme types
typedef enum {
    THEME_DARK = 0,
    THEME_LIGHT,
    THEME_CUSTOM
} ThemeType_t;

// Theme color identifiers
typedef enum {
    THEME_COLOR_BACKGROUND = 0,
    THEME_COLOR_FOREGROUND,
    THEME_COLOR_PRIMARY,
    THEME_COLOR_SECONDARY,
    THEME_COLOR_ACCENT,
    THEME_COLOR_SUCCESS,
    THEME_COLOR_WARNING,
    THEME_COLOR_ERROR,
    THEME_COLOR_TEXT,
    THEME_COLOR_TEXT_SECONDARY,
    THEME_COLOR_BORDER,
    THEME_COLOR_SHADOW,
    THEME_COLOR_COUNT
} ThemeColorId_t;

/**
 * @brief Initialize theme manager
 * @return 0 on success, negative on error
 */
int theme_manager_init(void);

/**
 * @brief Set active theme
 * @param theme Theme type to activate
 * @return 0 on success, negative on error
 */
int theme_set(ThemeType_t theme);

/**
 * @brief Get current theme
 * @return Current theme type
 */
ThemeType_t theme_get(void);

/**
 * @brief Get color for theme element
 * @param color_id Color identifier
 * @return RGB565 color value
 */
uint16_t theme_get_color(ThemeColorId_t color_id);

/**
 * @brief Set custom color
 * @param color_id Color identifier
 * @param color RGB565 color value
 * @return 0 on success, negative on error
 */
int theme_set_custom_color(ThemeColorId_t color_id, uint16_t color);

#endif // THEME_MANAGER_H
