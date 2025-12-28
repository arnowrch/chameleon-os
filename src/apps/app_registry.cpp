/**
 * @file app_registry.cpp
 * @brief Central registry of all 33 apps
 * 
 * Defines and registers all apps across 6 sectors
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>

// Sector 1: Productivity (6 apps)
extern App_t app_home;
extern App_t app_clock;
extern App_t app_calendar;
extern App_t app_notes;
extern App_t app_tasks;
extern App_t app_timer;

// Sector 2: Communication (6 apps)
extern App_t app_wifi;
extern App_t app_weather;
extern App_t app_news;
extern App_t app_email;
extern App_t app_messenger;
extern App_t app_social;

// Sector 3: Media (5 apps)
extern App_t app_music;
extern App_t app_photos;
extern App_t app_video;
extern App_t app_radio;
extern App_t app_podcast;

// Sector 4: Tools (6 apps)
extern App_t app_files;
extern App_t app_settings;
extern App_t app_monitor;
extern App_t app_terminal;
extern App_t app_editor;
extern App_t app_packages;

// Sector 5: Lifestyle (5 apps)
extern App_t app_fitness;
extern App_t app_health;
extern App_t app_sleep;
extern App_t app_meditation;
extern App_t app_habits;

// Sector 6: Games (5 apps)
extern App_t app_puzzle;
extern App_t app_retro;
extern App_t app_drawing;
extern App_t app_animations;
extern App_t app_screensaver;

/**
 * @brief Register all 33 apps
 * @return Number of apps registered
 */
int register_all_apps(void) {
    int count = 0;
    
    // Sector 1: Productivity
    count += (app_register(&app_home) >= 0) ? 1 : 0;
    count += (app_register(&app_clock) >= 0) ? 1 : 0;
    count += (app_register(&app_calendar) >= 0) ? 1 : 0;
    count += (app_register(&app_notes) >= 0) ? 1 : 0;
    count += (app_register(&app_tasks) >= 0) ? 1 : 0;
    count += (app_register(&app_timer) >= 0) ? 1 : 0;
    
    // Sector 2: Communication
    count += (app_register(&app_wifi) >= 0) ? 1 : 0;
    count += (app_register(&app_weather) >= 0) ? 1 : 0;
    count += (app_register(&app_news) >= 0) ? 1 : 0;
    count += (app_register(&app_email) >= 0) ? 1 : 0;
    count += (app_register(&app_messenger) >= 0) ? 1 : 0;
    count += (app_register(&app_social) >= 0) ? 1 : 0;
    
    // Sector 3: Media
    count += (app_register(&app_music) >= 0) ? 1 : 0;
    count += (app_register(&app_photos) >= 0) ? 1 : 0;
    count += (app_register(&app_video) >= 0) ? 1 : 0;
    count += (app_register(&app_radio) >= 0) ? 1 : 0;
    count += (app_register(&app_podcast) >= 0) ? 1 : 0;
    
    // Sector 4: Tools
    count += (app_register(&app_files) >= 0) ? 1 : 0;
    count += (app_register(&app_settings) >= 0) ? 1 : 0;
    count += (app_register(&app_monitor) >= 0) ? 1 : 0;
    count += (app_register(&app_terminal) >= 0) ? 1 : 0;
    count += (app_register(&app_editor) >= 0) ? 1 : 0;
    count += (app_register(&app_packages) >= 0) ? 1 : 0;
    
    // Sector 5: Lifestyle
    count += (app_register(&app_fitness) >= 0) ? 1 : 0;
    count += (app_register(&app_health) >= 0) ? 1 : 0;
    count += (app_register(&app_sleep) >= 0) ? 1 : 0;
    count += (app_register(&app_meditation) >= 0) ? 1 : 0;
    count += (app_register(&app_habits) >= 0) ? 1 : 0;
    
    // Sector 6: Games
    count += (app_register(&app_puzzle) >= 0) ? 1 : 0;
    count += (app_register(&app_retro) >= 0) ? 1 : 0;
    count += (app_register(&app_drawing) >= 0) ? 1 : 0;
    count += (app_register(&app_animations) >= 0) ? 1 : 0;
    count += (app_register(&app_screensaver) >= 0) ? 1 : 0;
    
    return count;
}
