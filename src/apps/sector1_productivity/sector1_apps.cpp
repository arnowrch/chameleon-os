/**
 * @file sector1_apps.cpp
 * @brief Remaining Sector 1 (Productivity) apps: Calendar, Notes, Tasks, Timer
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>

// ============================================================================
// Calendar App
// ============================================================================
static void calendar_init(void) {}
static void calendar_on_enter(void) {}
static void calendar_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(90, 50, "Calendar", primary, 2);
    display_hal_text(60, 80, "Monthly View", fg, 1);
}
static void calendar_on_exit(void) {}
static void calendar_cleanup(void) {}

App_t app_calendar = {
    .name = "Calendar",
    .icon = "calendar",
    .sector = 1,
    .init = calendar_init,
    .on_enter = calendar_on_enter,
    .on_update = calendar_on_update,
    .on_exit = calendar_on_exit,
    .cleanup = calendar_cleanup
};

// ============================================================================
// Notes App
// ============================================================================
static void notes_init(void) {}
static void notes_on_enter(void) {}
static void notes_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(100, 50, "Notes", primary, 2);
    display_hal_text(50, 80, "Quick Notes App", fg, 1);
}
static void notes_on_exit(void) {}
static void notes_cleanup(void) {}

App_t app_notes = {
    .name = "Notes",
    .icon = "notes",
    .sector = 1,
    .init = notes_init,
    .on_enter = notes_on_enter,
    .on_update = notes_on_update,
    .on_exit = notes_on_exit,
    .cleanup = notes_cleanup
};

// ============================================================================
// Tasks App
// ============================================================================
static void tasks_init(void) {}
static void tasks_on_enter(void) {}
static void tasks_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(100, 50, "Tasks", primary, 2);
    display_hal_text(50, 80, "To-Do Manager", fg, 1);
}
static void tasks_on_exit(void) {}
static void tasks_cleanup(void) {}

App_t app_tasks = {
    .name = "Tasks",
    .icon = "tasks",
    .sector = 1,
    .init = tasks_init,
    .on_enter = tasks_on_enter,
    .on_update = tasks_on_update,
    .on_exit = tasks_on_exit,
    .cleanup = tasks_cleanup
};

// ============================================================================
// Timer App
// ============================================================================
static void timer_init(void) {}
static void timer_on_enter(void) {}
static void timer_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(100, 50, "Timer", primary, 2);
    display_hal_text(40, 80, "Countdown Timer", fg, 1);
}
static void timer_on_exit(void) {}
static void timer_cleanup(void) {}

App_t app_timer = {
    .name = "Timer",
    .icon = "timer",
    .sector = 1,
    .init = timer_init,
    .on_enter = timer_on_enter,
    .on_update = timer_on_update,
    .on_exit = timer_on_exit,
    .cleanup = timer_cleanup
};
