/**
 * @file ui_framework.h
 * @brief Focus-Carousel UI framework
 * 
 * Innovative navigation with horizontal carousel and focus indicator
 */

#ifndef UI_FRAMEWORK_H
#define UI_FRAMEWORK_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize UI framework
 * @return 0 on success, negative on error
 */
int ui_framework_init(void);

/**
 * @brief Navigate to previous app
 */
void ui_navigate_prev(void);

/**
 * @brief Navigate to next app
 */
void ui_navigate_next(void);

/**
 * @brief Render current UI frame
 */
void ui_render(void);

/**
 * @brief Process touch input
 * @param x X coordinate
 * @param y Y coordinate
 * @param pressed Touch state
 */
void ui_process_touch(int x, int y, bool pressed);

#endif // UI_FRAMEWORK_H
