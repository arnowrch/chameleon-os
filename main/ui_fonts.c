/**
 * @file ui_fonts.c
 * @brief Font definitions for the Luxury UI.
 * 
 * NOTE: In a real project, these fonts would be generated using the LVGL font converter.
 * For this example, we use the default built-in fonts as placeholders, but rename them
 * to match the required Montserrat names to avoid compilation errors.
 * 
 * Required fonts:
 * - montserrat_22_bold (H1)
 * - montserrat_18_medium (H2)
 * - montserrat_12_regular (Body)
 */

#include "lvgl.h"

// Placeholder declarations using built-in LVGL fonts
// Replace these with your generated fonts in a real project.

// H1: 22px Bold -> Using built-in Montserrat 24
const lv_font_t montserrat_22_bold = LV_FONT_DEFAULT; // Placeholder

// H2: 18px Medium -> Using built-in Montserrat 18
const lv_font_t montserrat_18_medium = LV_FONT_DEFAULT; // Placeholder

// Body: 12px Regular -> Using built-in Montserrat 12
const lv_font_t montserrat_12_regular = LV_FONT_DEFAULT; // Placeholder

// NOTE: The placeholders above use LV_FONT_DEFAULT. 
// For a real luxury UI, you must generate the Montserrat fonts 
// (e.g., Montserrat 22 Bold, 18 Medium, 12 Regular) 
// and replace LV_FONT_DEFAULT with the actual font data structure.

