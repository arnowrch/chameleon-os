/**
 * @file sector3_apps.cpp
 * @brief Sector 3 (Media) apps
 */

#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"
#include <Arduino.h>

// Music Player
static void music_init(void) {}
static void music_on_enter(void) {}
static void music_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Music", primary, 2);
    display_hal_text(60, 80, "Music Player", fg, 1);
}
static void music_on_exit(void) {}
static void music_cleanup(void) {}
App_t app_music = {.name = "Music", .icon = "music", .sector = 3,
    .init = music_init, .on_enter = music_on_enter,
    .on_update = music_on_update, .on_exit = music_on_exit, .cleanup = music_cleanup};

// Photos
static void photos_init(void) {}
static void photos_on_enter(void) {}
static void photos_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Photos", primary, 2);
    display_hal_text(50, 80, "Photo Gallery", fg, 1);
}
static void photos_on_exit(void) {}
static void photos_cleanup(void) {}
App_t app_photos = {.name = "Photos", .icon = "photos", .sector = 3,
    .init = photos_init, .on_enter = photos_on_enter,
    .on_update = photos_on_update, .on_exit = photos_on_exit, .cleanup = photos_cleanup};

// Video
static void video_init(void) {}
static void video_on_enter(void) {}
static void video_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Video", primary, 2);
    display_hal_text(50, 80, "Video Player", fg, 1);
}
static void video_on_exit(void) {}
static void video_cleanup(void) {}
App_t app_video = {.name = "Video", .icon = "video", .sector = 3,
    .init = video_init, .on_enter = video_on_enter,
    .on_update = video_on_update, .on_exit = video_on_exit, .cleanup = video_cleanup};

// Radio
static void radio_init(void) {}
static void radio_on_enter(void) {}
static void radio_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Radio", primary, 2);
    display_hal_text(40, 80, "Internet Radio", fg, 1);
}
static void radio_on_exit(void) {}
static void radio_cleanup(void) {}
App_t app_radio = {.name = "Radio", .icon = "radio", .sector = 3,
    .init = radio_init, .on_enter = radio_on_enter,
    .on_update = radio_on_update, .on_exit = radio_on_exit, .cleanup = radio_cleanup};

// Podcast
static void podcast_init(void) {}
static void podcast_on_enter(void) {}
static void podcast_on_update(void) {
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    display_hal_text(80, 50, "Podcast", primary, 2);
    display_hal_text(40, 80, "Podcast Player", fg, 1);
}
static void podcast_on_exit(void) {}
static void podcast_cleanup(void) {}
App_t app_podcast = {.name = "Podcast", .icon = "podcast", .sector = 3,
    .init = podcast_init, .on_enter = podcast_on_enter,
    .on_update = podcast_on_update, .on_exit = podcast_on_exit, .cleanup = podcast_cleanup};
