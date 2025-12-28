/**
 * @file sector5_apps.cpp
 * @brief Sector 5 (Lifestyle) apps
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>

// Fitness Tracker
static void fitness_init(void) {}
static void fitness_on_enter(void) {}
static void fitness_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Fitness", primary, 2);
    display_hal_text(40, 80, "Activity Tracker", fg, 1);
}
static void fitness_on_exit(void) {}
static void fitness_cleanup(void) {}
App_t app_fitness = {.name = "Fitness", .icon = "fitness", .sector = 5,
    .init = fitness_init, .on_enter = fitness_on_enter,
    .on_update = fitness_on_update, .on_exit = fitness_on_exit, .cleanup = fitness_cleanup};

// Health Monitor
static void health_init(void) {}
static void health_on_enter(void) {}
static void health_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Health", primary, 2);
    display_hal_text(40, 80, "Health Monitor", fg, 1);
}
static void health_on_exit(void) {}
static void health_cleanup(void) {}
App_t app_health = {.name = "Health", .icon = "health", .sector = 5,
    .init = health_init, .on_enter = health_on_enter,
    .on_update = health_on_update, .on_exit = health_on_exit, .cleanup = health_cleanup};

// Sleep Tracker
static void sleep_init(void) {}
static void sleep_on_enter(void) {}
static void sleep_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Sleep", primary, 2);
    display_hal_text(40, 80, "Sleep Tracker", fg, 1);
}
static void sleep_on_exit(void) {}
static void sleep_cleanup(void) {}
App_t app_sleep = {.name = "Sleep", .icon = "sleep", .sector = 5,
    .init = sleep_init, .on_enter = sleep_on_enter,
    .on_update = sleep_on_update, .on_exit = sleep_on_exit, .cleanup = sleep_cleanup};

// Meditation
static void meditation_init(void) {}
static void meditation_on_enter(void) {}
static void meditation_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(70, 50, "Meditation", primary, 2);
    display_hal_text(50, 80, "Mindfulness", fg, 1);
}
static void meditation_on_exit(void) {}
static void meditation_cleanup(void) {}
App_t app_meditation = {.name = "Meditation", .icon = "zen", .sector = 5,
    .init = meditation_init, .on_enter = meditation_on_enter,
    .on_update = meditation_on_update, .on_exit = meditation_on_exit, .cleanup = meditation_cleanup};

// Habit Tracker
static void habits_init(void) {}
static void habits_on_enter(void) {}
static void habits_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Habits", primary, 2);
    display_hal_text(40, 80, "Habit Tracker", fg, 1);
}
static void habits_on_exit(void) {}
static void habits_cleanup(void) {}
App_t app_habits = {.name = "Habits", .icon = "habits", .sector = 5,
    .init = habits_init, .on_enter = habits_on_enter,
    .on_update = habits_on_update, .on_exit = habits_on_exit, .cleanup = habits_cleanup};
