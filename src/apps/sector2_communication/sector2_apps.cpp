/**
 * @file sector2_apps.cpp
 * @brief Remaining Sector 2 (Communication) apps
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>

// Weather
static void weather_init(void) {}
static void weather_on_enter(void) {}
static void weather_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(90, 50, "Weather", primary, 2);
    display_hal_text(70, 80, "Forecast", fg, 1);
}
static void weather_on_exit(void) {}
static void weather_cleanup(void) {}
App_t app_weather = {.name = "Weather", .icon = "weather", .sector = 2,
    .init = weather_init, .on_enter = weather_on_enter,
    .on_update = weather_on_update, .on_exit = weather_on_exit, .cleanup = weather_cleanup};

// News
static void news_init(void) {}
static void news_on_enter(void) {}
static void news_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(100, 50, "News", primary, 2);
    display_hal_text(60, 80, "News Feed", fg, 1);
}
static void news_on_exit(void) {}
static void news_cleanup(void) {}
App_t app_news = {.name = "News", .icon = "news", .sector = 2,
    .init = news_init, .on_enter = news_on_enter,
    .on_update = news_on_update, .on_exit = news_on_exit, .cleanup = news_cleanup};

// Email
static void email_init(void) {}
static void email_on_enter(void) {}
static void email_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(100, 50, "Email", primary, 2);
    display_hal_text(50, 80, "Email Client", fg, 1);
}
static void email_on_exit(void) {}
static void email_cleanup(void) {}
App_t app_email = {.name = "Email", .icon = "email", .sector = 2,
    .init = email_init, .on_enter = email_on_enter,
    .on_update = email_on_update, .on_exit = email_on_exit, .cleanup = email_cleanup};

// Messenger
static void messenger_init(void) {}
static void messenger_on_enter(void) {}
static void messenger_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Messenger", primary, 2);
    display_hal_text(60, 80, "Messages", fg, 1);
}
static void messenger_on_exit(void) {}
static void messenger_cleanup(void) {}
App_t app_messenger = {.name = "Messenger", .icon = "msg", .sector = 2,
    .init = messenger_init, .on_enter = messenger_on_enter,
    .on_update = messenger_on_update, .on_exit = messenger_on_exit, .cleanup = messenger_cleanup};

// Social Hub
static void social_init(void) {}
static void social_on_enter(void) {}
static void social_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(70, 50, "Social Hub", primary, 2);
    display_hal_text(60, 80, "Social Feed", fg, 1);
}
static void social_on_exit(void) {}
static void social_cleanup(void) {}
App_t app_social = {.name = "Social", .icon = "social", .sector = 2,
    .init = social_init, .on_enter = social_on_enter,
    .on_update = social_on_update, .on_exit = social_on_exit, .cleanup = social_cleanup};
