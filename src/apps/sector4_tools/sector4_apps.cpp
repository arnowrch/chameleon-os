/**
 * @file sector4_apps.cpp
 * @brief Sector 4 (Tools) apps
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>

// File Manager
static void files_init(void) {}
static void files_on_enter(void) {}
static void files_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(60, 50, "File Manager", primary, 2);
    display_hal_text(70, 80, "Browse Files", fg, 1);
}
static void files_on_exit(void) {}
static void files_cleanup(void) {}
App_t app_files = {.name = "Files", .icon = "files", .sector = 4,
    .init = files_init, .on_enter = files_on_enter,
    .on_update = files_on_update, .on_exit = files_on_exit, .cleanup = files_cleanup};

// Settings
static void settings_init(void) {}
static void settings_on_enter(void) {}
static void settings_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Settings", primary, 2);
    display_hal_text(50, 80, "System Config", fg, 1);
}
static void settings_on_exit(void) {}
static void settings_cleanup(void) {}
App_t app_settings = {.name = "Settings", .icon = "settings", .sector = 4,
    .init = settings_init, .on_enter = settings_on_enter,
    .on_update = settings_on_update, .on_exit = settings_on_exit, .cleanup = settings_cleanup};

// System Monitor
static void monitor_init(void) {}
static void monitor_on_enter(void) {}
static void monitor_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(60, 50, "Sys Monitor", primary, 2);
    display_hal_text(40, 80, "CPU/RAM/Storage", fg, 1);
}
static void monitor_on_exit(void) {}
static void monitor_cleanup(void) {}
App_t app_monitor = {.name = "Monitor", .icon = "monitor", .sector = 4,
    .init = monitor_init, .on_enter = monitor_on_enter,
    .on_update = monitor_on_update, .on_exit = monitor_on_exit, .cleanup = monitor_cleanup};

// Terminal
static void terminal_init(void) {}
static void terminal_on_enter(void) {}
static void terminal_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Terminal", primary, 2);
    display_hal_text(40, 80, "Command Line", fg, 1);
}
static void terminal_on_exit(void) {}
static void terminal_cleanup(void) {}
App_t app_terminal = {.name = "Terminal", .icon = "term", .sector = 4,
    .init = terminal_init, .on_enter = terminal_on_enter,
    .on_update = terminal_on_update, .on_exit = terminal_on_exit, .cleanup = terminal_cleanup};

// Code Editor
static void editor_init(void) {}
static void editor_on_enter(void) {}
static void editor_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(90, 50, "Editor", primary, 2);
    display_hal_text(50, 80, "Code Editor", fg, 1);
}
static void editor_on_exit(void) {}
static void editor_cleanup(void) {}
App_t app_editor = {.name = "Editor", .icon = "editor", .sector = 4,
    .init = editor_init, .on_enter = editor_on_enter,
    .on_update = editor_on_update, .on_exit = editor_on_exit, .cleanup = editor_cleanup};

// Package Manager
static void packages_init(void) {}
static void packages_on_enter(void) {}
static void packages_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(70, 50, "Packages", primary, 2);
    display_hal_text(40, 80, "Package Manager", fg, 1);
}
static void packages_on_exit(void) {}
static void packages_cleanup(void) {}
App_t app_packages = {.name = "Packages", .icon = "pkg", .sector = 4,
    .init = packages_init, .on_enter = packages_on_enter,
    .on_update = packages_on_update, .on_exit = packages_on_exit, .cleanup = packages_cleanup};
