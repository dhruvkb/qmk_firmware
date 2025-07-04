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
