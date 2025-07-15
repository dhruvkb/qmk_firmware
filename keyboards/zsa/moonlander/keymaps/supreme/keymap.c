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

// Implementation
// ==============

enum tap_dance_codes {
    LAYER_1,
    LAYER_2,
};

static tap dance_state[2]; // as many as the entries in `tap_dance_codes`

void dance_layer_1_finished(tap_dance_state_t *state, void *user_data) {
    dance_state[0].step = dance_step(state);
    switch (dance_state[0].step) {
        case DOUBLE_TAP:
            layer_move(1);
            break;
    }
}

void dance_layer_1_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10); // don't know why
    switch (dance_state[0].step) {}
    dance_state[0].step = 0;
}

void dance_layer_2_finished(tap_dance_state_t *state, void *user_data) {
    dance_state[1].step = dance_step(state);
    switch (dance_state[1].step) {
        case DOUBLE_TAP:
            layer_move(2);
            break;
    }
}

void dance_layer_2_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10); // don't know why
    switch (dance_state[1].step) {}
    dance_state[1].step = 0;
}

tap_dance_action_t tap_dance_actions[] = {
    [LAYER_1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_layer_1_finished, dance_layer_1_reset),
    [LAYER_2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_layer_2_finished, dance_layer_2_reset),
};

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
        /* left orange  */ {COL_WHT},
        /* column 14    */ {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLU},
        /* column 13    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLU},
        /* column 12    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLU},
        /* column 11    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column 10    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  9    */ {COL_AQU}, {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* column  8    */ {COL_RED}, {COL_BLK}, {COL_BLK},
        /* right thumb  */ {COL_BLK}, {COL_BLK}, {COL_BLK},
        /* right orange */ {COL_WHT}
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

// Color the modifier keys based on whether they are active.
void handle_mod_color(uint8_t mods, uint8_t r, uint8_t g, uint8_t b) {
    // Ctrl ⌃ + Cmd ⌘ is used as the Raycast shortcut, mapped to the left orange key.
    if (mods & MOD_MASK_CTRL && mods & MOD_MASK_GUI && !(mods & MOD_MASK_ALT || mods & MOD_MASK_SHIFT)) {
        rgb_matrix_set_color(35, COL_WHT);
        return; // Do not light up the individual modifier keys.
    }
    // Ctrl ⌃ + Opt ⌥ is used for window management, mapped to the right orange key.
    if (mods & MOD_MASK_CTRL && mods & MOD_MASK_ALT && !(mods & MOD_MASK_GUI || mods & MOD_MASK_SHIFT)) {
        rgb_matrix_set_color(71, COL_WHT);
        return; // Do not light up the individual modifier keys.
    }
    if (mods & MOD_MASK_SHIFT) {
        // Shift ⇧ is present in two locations to afford right handed mouse usage.
        rgb_matrix_set_color(3, r, g, b);  // column 1
        rgb_matrix_set_color(69, r, g, b); // right thumb cluster
    }
    if (mods & MOD_MASK_ALT) {
        // Opt ⌥ is present in two locations to afford right handed mouse usage.
        rgb_matrix_set_color(4, r, g, b);  // column 1
        rgb_matrix_set_color(70, r, g, b); // right thumb cluster
    }
    if (mods & MOD_MASK_GUI) {
        // Cmd ⌘ is present in two locations to enable key combinations with arrows.
        rgb_matrix_set_color(33, r, g, b); // left thumb cluster
        rgb_matrix_set_color(60, r, g, b); // column 10
    }
    if (mods & MOD_MASK_CTRL) {
        rgb_matrix_set_color(34, r, g, b);
    }
}

// Handle custom RGB matrix colors.
bool rgb_matrix_indicators_user(void) {
    if (rawhid_state.rgb_control) {
        // An external application is controlling the RGB.
        return false;
    }
    if (keyboard_config.disable_layer_led) {
        // Layer specific colors are disabled.
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
    handle_mod_color(get_oneshot_mods(), COL_GRN);
    handle_mod_color(get_mods(), COL_BLU);

    return true;
}

//  ____                                     _
// |  _ \ __ _ ___ _____      _____  _ __ __| |___
// | |_) / _` / __/ __\ \ /\ / / _ \| '__/ _` / __|
// |  __/ (_| \__ \__ \\ V  V / (_) | | | (_| \__ \.
// |_|   \__,_|___/___/ \_/\_/ \___/|_|  \__,_|___/

#ifdef OS_PASSWORD
    static const char os_password[] PROGMEM = OS_PASSWORD;
#else
    #error "OS_PASSWORD must be defined in environment variables."
#endif

#ifdef OP_PASSWORD
    static const char op_password[] PROGMEM = OP_PASSWORD;
#else
    #error "OP_PASSWORD must be defined in environment variables."
#endif

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

// Custom
// ======

enum custom_keycodes {
    RGB_SLD = SAFE_RANGE,
    NO_MOD,
    OS_PASS,
    OP_PASS,
    BR_ROUND,
    BR_SQUAR,
    BR_ANGLE,
    BR_CURLY,
};

// Handle custom key presses.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_SLD:
            if (rawhid_state.rgb_control) {
                // An external application is controlling the RGB.
                return false;
            }
            if (record->event.pressed) {
                rgblight_mode(1);
            }
            return false;
        case NO_MOD:
            if (record->event.pressed) {
                clear_oneshot_mods(); // Clear all active one-shot mods.
                clear_mods(); // Clear all active regular mods.
                send_keyboard_report();
            }
            return false;
        case OS_PASS:
            if (record->event.pressed) {
                SEND_STRING(os_password);
                // Press enter to submit the password.
                SEND_STRING(SS_TAP(X_ENTER));
            }
            return false;
        case OP_PASS:
            if (record->event.pressed) {
                SEND_STRING(op_password);
                // Press enter to submit the password.
                SEND_STRING(SS_TAP(X_ENTER));
            }
            return false;
        case BR_ROUND:
            if (record->event.pressed) {
                SEND_STRING("()" SS_TAP(X_LEFT));
            }
            return false;
        case BR_SQUAR:
            if (record->event.pressed) {
                SEND_STRING("[]" SS_TAP(X_LEFT));
            }
            return false;
        case BR_ANGLE:
            if (record->event.pressed) {
                SEND_STRING("<>" SS_TAP(X_LEFT));
            }
            return false;
        case BR_CURLY:
            if (record->event.pressed) {
                SEND_STRING("{}" SS_TAP(X_LEFT));
            }
            return false;
    }
    return true;
}

// Handle modifier key releases to ensure they are unregistered properly.
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case OSM(MOD_LALT):
        case OSM(MOD_LSFT):
        case OSM(MOD_LCTL):
        case OSM(MOD_LGUI): {
            uint8_t mask = 0;
            if (keycode == OSM(MOD_LALT))  mask = MOD_MASK_ALT;
            if (keycode == OSM(MOD_LSFT))  mask = MOD_MASK_SHIFT;
            if (keycode == OSM(MOD_LCTL))  mask = MOD_MASK_CTRL;
            if (keycode == OSM(MOD_LGUI))  mask = MOD_MASK_GUI;
            if (!record->event.pressed && (keyboard_report->mods & mask)) {
                unregister_mods(mask);
            }
            break;
        }
    }
}

// TODO: Update keycodes for mouse keys when ZSA's fork of QMK is updated.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_moonlander(
        KC_GRV,             KC_1,          KC_2,          KC_3,          KC_4,          KC_5,          TD(LAYER_1),   /* row 1 */ TD(LAYER_2),   KC_6,          KC_7,          KC_8,          KC_9,          KC_0,          KC_MINS,
        KC_TAB,             KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,          NO_MOD,        /* row 2 */ KC_EQL,        KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,          KC_BSLS,
        CW_TOGG,            KC_A,          KC_S,          KC_D,          KC_F,          KC_G,          KC_ESC,        /* row 3 */ KC_DEL,        KC_H,          KC_J,          KC_K,          KC_L,          KC_SCLN,       KC_QUOT,
        OSM(MOD_LSFT),      KC_Z,          KC_X,          KC_C,          KC_V,          KC_B,                         /* row 4 */                KC_N,          KC_M,          KC_COMM,       KC_DOT,        KC_UP,         KC_ENT,
        OSM(MOD_LALT),      BR_ROUND,      BR_CURLY,      BR_SQUAR,      BR_ANGLE,      /* orange → */ RAYCAST,       /* row 5 */ WIN_MGMT,      /* ← orange */ OSM(MOD_LGUI), KC_SLSH,       KC_LEFT,       KC_DOWN,       KC_RIGHT,
                                                                         KC_SPC,        OSM(MOD_LGUI), OSM(MOD_LCTL), /* thumb */ OSM(MOD_LALT), OSM(MOD_LSFT), KC_BSPC
    ),
    [1] = LAYOUT_moonlander(
        _______,            KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_F5,         TO(0),         /* row 1 */ TO(0),         KC_F6,         KC_F7,         KC_F8,         KC_F9,         KC_F10,        _______,
        _______,            _______,       _______,       _______,       KC_VOLU,       KC_F15,        _______,       /* row 2 */ _______,       _______,       _______,       _______,       _______,       _______,       _______,
        _______,            _______,       _______,       _______,       KC_VOLD,       KC_F14,        _______,       /* row 3 */ _______,       _______,       _______,       _______,       _______,       _______,       _______,
        _______,            _______,       _______,       _______,       KC_MUTE,       _______,                      /* row 4 */                _______,       _______,       _______,       _______,       _______,       _______,
        _______,            _______,       _______,       _______,       _______,       /* orange → */ OS_PASS,       /* row 5 */ OP_PASS,       /* ← orange */ _______,       _______,       KC_MPRV,       KC_MPLY,       KC_MNXT,
                                                                         _______,       _______,       _______,       /* thumb */ _______,       _______,       _______
    ),
    [2] = LAYOUT_moonlander(
        AU_TOGG,            MU_TOGG,       _______,       _______,       _______,       _______,       TO(0),         /* row 1 */ TO(0),         _______,       _______,       _______,       _______,       _______,       QK_BOOT,
        TOGGLE_LAYER_COLOR, _______,       RGB_SPI,       RGB_HUI,       RGB_SAI,       RGB_VAI,       _______,       /* row 2 */ _______,       _______,       _______,       _______,       _______,       _______,       _______,
        RGB_TOG,            _______,       RGB_SPD,       RGB_HUD,       RGB_SAD,       RGB_VAD,       _______,       /* row 3 */ _______,       _______,       _______,       _______,       _______,       _______,       _______,
        RGB_SLD,            _______,       _______,       _______,       _______,       _______,                      /* row 4 */                _______,       _______,       KC_WH_U,       KC_BTN1,       KC_MS_U,       KC_BTN2,
        RGB_MODE_FORWARD,   _______,       _______,       _______,       _______,       /* orange → */ DESK_UP,       /* row 5 */ DESK_DOWN,     /* ← orange */ _______,       KC_WH_D,       KC_MS_L,       KC_MS_D,       KC_MS_R,
                                                                         _______,       _______,       _______,       /* thumb */ _______,       _______,       _______
    ),
};
