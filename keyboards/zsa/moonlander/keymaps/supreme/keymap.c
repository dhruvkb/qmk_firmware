#include QMK_KEYBOARD_H
#include "version.h"

//  _____                 _
// |_   _|_ _ _ __     __| | __ _ _ __   ___ ___
//   | |/ _` | '_ \   / _` |/ _` | '_ \ / __/ _ \.
//   | | (_| | |_) | | (_| | (_| | | | | (_|  __/
//   |_|\__,_| .__/   \__,_|\__,_|_| |_|\___\___|
//           |_|

// Helpers
// =======

typedef struct {
    bool    is_press_action;
    uint8_t step;
} tap;

enum { SINGLE_TAP = 1, SINGLE_HOLD, DOUBLE_TAP, DOUBLE_HOLD, DOUBLE_SINGLE_TAP, MORE_TAPS };

uint8_t dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed)
            return SINGLE_TAP;
        else
            return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted)
            return DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return DOUBLE_HOLD;
        else
            return DOUBLE_TAP;
    }
    return MORE_TAPS;
}

//  ____   ____ ____
// |  _ \ / ___| __ )
// | |_) | |  _|  _ \.
// |  _ <| |_| | |_) |
// |_| \_\\____|____/

// Boilerplate setup
// =================

extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

// Color definitions
// =================

// Tones
#define COL_BLK 0x00, 0x00, 0x00
#define COL_WHT 0xff, 0xff, 0xff

// Primary colors
#define COL_RED 0xff, 0x00, 0x00
#define COL_GRN 0x00, 0xff, 0x00
#define COL_BLU 0x00, 0x00, 0xff

// Secondary colors
#define COL_YLO 0xff, 0xff, 0x00
#define COL_MAG 0xff, 0x00, 0xff
#define COL_AQU 0x00, 0xff, 0xff

// Tertiary colors
#define COL_RNG 0xff, 0x7f, 0x00
#define COL_CTR 0x7f, 0xff, 0x00
#define COL_VIO 0x7f, 0x00, 0xff
#define COL_AZR 0x00, 0x7f, 0xff

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [1] = {
        /* column  1    */ {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  2    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  3    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  4    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  5    */ {COL_AQU}, {COL_RNG}, {COL_RNG}, {COL_RNG}, {COL_BLK},
        /* column  6    */ {COL_AQU}, {COL_YLO}, {COL_YLO}, {COL_BLK},
        /* column  7    */ {COL_GRN}, {COL_BLK}, {COL_BLK},
        /* left thumb   */ {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* left orange  */ {COL_BLK},
        /* column 14    */ {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLU},
        /* column 13    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLU},
        /* column 12    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLU},
        /* column 11    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column 10    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  9    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  8    */ {COL_RED}, {COL_BLK}, {COL_BLK},
        /* right thumb  */ {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* right orange */ {COL_BLK}
    },
    [2] = {
        /* column  1    */ {COL_CTR}, {COL_VIO}, {COL_VIO}, {COL_BLK}, {COL_VIO},
        /* column  2    */ {COL_CTR}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  3    */ {COL_BLK}, {COL_VIO}, {COL_VIO}, {COL_BLK}, {COL_BLK},
        /* column  4    */ {COL_BLK}, {COL_VIO}, {COL_VIO}, {COL_BLK}, {COL_BLK},
        /* column  5    */ {COL_BLK}, {COL_VIO}, {COL_VIO}, {COL_BLK}, {COL_BLK},
        /* column  6    */ {COL_BLK}, {COL_VIO}, {COL_VIO}, {COL_BLK},
        /* column  7    */ {COL_RED}, {COL_BLK}, {COL_BLK},
        /* left thumb   */ {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* left orange  */ {COL_WHT},
        /* column 14    */ {COL_RED}, {COL_BLK}, {COL_BLK}, {COL_AQU}, {COL_AQU},
        /* column 13    */ {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_AQU}, {COL_AQU},
        /* column 12    */ {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_AQU}, {COL_AQU},
        /* column 11    */ {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_AQU}, {COL_AQU},
        /* column 10    */ {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  9    */ {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  8    */ {COL_GRN}, {COL_BLK}, {COL_BLK},
        /* right thumb  */ {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* right orange */ {COL_WHT}
    },
};

// Set the layer color from `ledmap`.
void set_layer_color(int layer) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        // TODO: `RGB` is deprecated. Switch to `rgb_t` when ZSA's fork of QMK is updated.
        RGB rgb = {
            .r = pgm_read_byte(&ledmap[layer][i][0]),
            .g = pgm_read_byte(&ledmap[layer][i][1]),
            .b = pgm_read_byte(&ledmap[layer][i][2]),
        };
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
}

// Color the "CAPS Word" key based on whether it is active.
void handle_caps_word_color(void) {
    if (is_caps_word_on()) {
        rgb_matrix_set_color(2, COL_GRN);
    }
}

// Color the one-shot modifier keys based on whether they are active.
void handle_osm_color(void) {
    uint8_t mods = get_oneshot_mods();

    // Ctrl ⌃ + Cmd ⌘ is used as the Raycast shortcut, mapped to the left orange key.
    if (mods & MOD_MASK_CTRL && mods & MOD_MASK_GUI) {
        rgb_matrix_set_color(35, COL_WHT);
        return; // Do not light up the individual modifier keys.
    }
    // Ctrl ⌃ + Opt ⌥ is used for window management, mapped to the right orange key.
    if (mods & MOD_MASK_CTRL && mods & MOD_MASK_ALT) {
        rgb_matrix_set_color(71, COL_WHT);
        return; // Do not light up the individual modifier keys.
    }
    if (mods & MOD_MASK_SHIFT) {
        // Shift ⇧ is present in two locations to afford right handed mouse usage.
        rgb_matrix_set_color(3, COL_GRN);  // column 1
        rgb_matrix_set_color(69, COL_GRN); // right thumb cluster
    }
    if (mods & MOD_MASK_ALT) {
        // Opt ⌥ is present in two locations to afford right handed mouse usage.
        rgb_matrix_set_color(4, COL_GRN);  // column 1
        rgb_matrix_set_color(70, COL_GRN); // right thumb cluster
    }
    if (mods & MOD_MASK_GUI) {
        rgb_matrix_set_color(33, COL_GRN);
    }
    if (mods & MOD_MASK_CTRL) {
        rgb_matrix_set_color(34, COL_GRN);
    }
}

// Handle custom RGB matrix colors.
bool rgb_matrix_indicators_user(void) {
    if (rawhid_state.rgb_control) {
        // An external application is controlling the RGB.
        return false;
    }
    if (keyboard_config.disable_layer_led) {
        return false;
    }
    switch (biton32(layer_state)) {
        case 1:
            set_layer_color(1);
            break;
        case 2:
            set_layer_color(2);
            break;
        default:
            if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
                // The keyboard has asked for the lights to be turned off.
                rgb_matrix_set_color_all(COL_BLK);
            }
            break;
    }

    handle_caps_word_color();
    handle_osm_color();

    return true;
}

//  _  __
// | |/ /___ _   _ _ __ ___   __ _ _ __
// | ' // _ \ | | | '_ ` _ \ / _` | '_ \.
// | . \  __/ |_| | | | | | | (_| | |_) |
// |_|\_\___|\__, |_| |_| |_|\__,_| .__/
//           |___/                |_|

// Shorthands
// ==========

// Layer 0
#define RAYCAST OSM(MOD_LCTL | MOD_LGUI) // Ctrl ⌃ + Cmd ⌘
#define WIN_MGMT OSM(MOD_LCTL | MOD_LALT) // Ctrl ⌃ + Opt ⌥

// Layer 1
#define DESK_UP LALT(LGUI(LCTL(KC_UP))) // Opt ⌥ + Cmd ⌘ + Ctrl ⌃ + Up Arrow
#define DESK_DOWN LALT(LGUI(LCTL(KC_DOWN))) // Opt ⌥ + Cmd ⌘ + Ctrl ⌃ + Down Arrow
