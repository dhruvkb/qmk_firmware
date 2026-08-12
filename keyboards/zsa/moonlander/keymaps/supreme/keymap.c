#include QMK_KEYBOARD_H
#include "version.h"
#include "qmk_map.h"

//  _   _ _   _ _
// | | | | |_(_) |___
// | | | | __| | / __|
// | |_| | |_| | \__ \.
//  \___/ \__|_|_|___/

// Clear active and regular mods and send a report.
static void clear_all_mods(void) {
    clear_oneshot_mods(); // Clear all active one-shot mods.
    clear_mods(); // Clear all active regular mods.

    send_keyboard_report();
}

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
    // Layers
    LAYER_1,
    LAYER_2,
    LAYER_3,

    // Brackets
    PRN,
    CBR,
    BRC,
    ABK,
};

static tap dance_state[4]; // one more than number of layers in `tap_dance_codes`

void dance_layer_finished(tap_dance_state_t *state, void *user_data, int layer) {
    clear_all_mods();

    dance_state[layer].step = dance_step(state);
    switch (dance_state[layer].step) {
        case SINGLE_TAP:
            // Arm a one-shot layer to be active for exactly the next keypress.
            set_oneshot_layer(layer, ONESHOT_START);
            break;
        case SINGLE_HOLD:
            layer_on(layer);
            break;
        case DOUBLE_TAP:
            layer_move(layer);
            break;
    }
}

void dance_layer_reset(tap_dance_state_t *state, void *user_data, int layer) {
    wait_ms(10); // don't know why
    switch (dance_state[layer].step) {
        case SINGLE_TAP:
            // Mark the one-shot layer as "primed" now that the key is up.
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case SINGLE_HOLD:
            layer_off(layer);
            break;
    }
    dance_state[layer].step = 0;
}

void dance_layer_1_finished(tap_dance_state_t *state, void *user_data) {
    dance_layer_finished(state, user_data, 1);
}

void dance_layer_1_reset(tap_dance_state_t *state, void *user_data) {
    dance_layer_reset(state, user_data, 1);
}

void dance_layer_2_finished(tap_dance_state_t *state, void *user_data) {
    dance_layer_finished(state, user_data, 2);
}

void dance_layer_2_reset(tap_dance_state_t *state, void *user_data) {
    dance_layer_reset(state, user_data, 2);
}

void dance_layer_3_finished(tap_dance_state_t *state, void *user_data) {
    dance_layer_finished(state, user_data, 3);
}

void dance_layer_3_reset(tap_dance_state_t *state, void *user_data) {
    dance_layer_reset(state, user_data, 3);
}

void dance_bracket(tap_dance_state_t *state, void *user_data, uint16_t left, uint16_t right) {
    uint8_t step = dance_step(state);
    switch (step) {
        case SINGLE_TAP:
            tap_code16(left);
            break;
        case SINGLE_HOLD:
            tap_code16(right);
            break;
    }
}

void dance_prn(tap_dance_state_t *state, void *user_data) {
    dance_bracket(state, user_data, KC_LPRN, KC_RPRN);
}

void dance_cbr(tap_dance_state_t *state, void *user_data) {
    dance_bracket(state, user_data, KC_LCBR, KC_RCBR);
}

void dance_brc(tap_dance_state_t *state, void *user_data) {
    dance_bracket(state, user_data, KC_LBRC, KC_RBRC);
}

void dance_abk(tap_dance_state_t *state, void *user_data) {
    dance_bracket(state, user_data, KC_LABK, KC_RABK);
}

tap_dance_action_t tap_dance_actions[] = {
    [LAYER_1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_layer_1_finished, dance_layer_1_reset),
    [LAYER_2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_layer_2_finished, dance_layer_2_reset),
    [LAYER_3] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_layer_3_finished, dance_layer_3_reset),

    [PRN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_prn, NULL),
    [CBR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_cbr, NULL),
    [BRC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_brc, NULL),
    [ABK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_abk, NULL),
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

// Map each physical LED to its position in `LAYOUT_moonlander`'s argument
// list. Lets `ledmap` be written in the same row-by-row order as the
// keymaps below; `set_layer_color` scatters the entries to the right LEDs
// at runtime.
//
// Hardware lays the LEDs out column-by-column (top to bottom on the left
// half, then rightmost-column to col 8 on the right half), with the thumbs
// and the wide "orange" key tacked on at the end of each half. The keymap,
// in contrast, is read row-by-row.
#define KEYS_PER_LAYER 72
static const uint8_t PROGMEM led_to_layout[RGB_MATRIX_LED_COUNT] = {
    // Left half.
     0, 14, 28, 42, 54, // col 1
     1, 15, 29, 43, 55, // col 2
     2, 16, 30, 44, 56, // col 3
     3, 17, 31, 45, 57, // col 4
     4, 18, 32, 46, 58, // col 5
     5, 19, 33, 47,     // col 6
     6, 20, 34,         // col 7
    66, 67, 68,         // left thumb
    59,                 // left orange
    // Right half.
    13, 27, 41, 53, 65, // col 14
    12, 26, 40, 52, 64, // col 13
    11, 25, 39, 51, 63, // col 12
    10, 24, 38, 50, 62, // col 11
     9, 23, 37, 49, 61, // col 10
     8, 22, 36, 48,     // col 9
     7, 21, 35,         // col 8
    71, 70, 69,         // right thumb
    60,                 // right orange
};

// Each entry corresponds to a key in `LAYOUT_moonlander` argument order
// (rows top-to-bottom, left half then right half, thumbs last), so the
// rows here line up visually with the keymaps below.
const uint8_t PROGMEM ledmap[][KEYS_PER_LAYER][3] = {
    [1] = {
        {COL_BLK},     {COL_WHT},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_RED},     /* row 1 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     /* row 2 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_BLK},     {COL_GRN},     {COL_GRN},     {COL_GRN},     {COL_GRN},     {COL_BLK},     {COL_BLK},     /* row 3 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},                    /* row 4 */                {COL_BLK},     {COL_BLK},     {COL_AZR},     {COL_AZR},     {COL_AZR},     {COL_AZR},
        {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_RED},     /* orange → */ {COL_WHT},     /* row 5 */ {COL_WHT},     /* ← orange */ {COL_RED},     {COL_AZR},     {COL_AZR},     {COL_AZR},     {COL_AZR},
                                                                    {COL_BLK},     {COL_BLK},     {COL_BLK},     /* thumb */ {COL_BLK},     {COL_BLK},     {COL_BLK}
    },
    [2] = {
        {COL_BLK},     {COL_BLK},     {COL_WHT},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_RED},     /* row 1 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_MAG},     {COL_AQU},     {COL_YLO},     /* row 2 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_MAG},     {COL_AQU},     {COL_YLO},     /* row 3 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_MAG},     {COL_AQU},                    /* row 4 */                {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_RED},     /* orange → */ {COL_WHT},     /* row 5 */ {COL_WHT},     /* ← orange */ {COL_RED},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
                                                                    {COL_BLK},     {COL_BLK},     {COL_BLK},     /* thumb */ {COL_BLK},     {COL_BLK},     {COL_BLK}
    },
    [3] = {
        {COL_CTR},     {COL_CTR},     {COL_BLK},     {COL_WHT},     {COL_BLK},     {COL_BLK},     {COL_RED},     /* row 1 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_RED},
        {COL_CTR},     {COL_BLK},     {COL_RNG},     {COL_RNG},     {COL_RNG},     {COL_RNG},     {COL_BLK},     /* row 2 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_CTR},     {COL_BLK},     {COL_RNG},     {COL_RNG},     {COL_RNG},     {COL_RNG},     {COL_BLK},     /* row 3 */ {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_VIO},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},                    /* row 4 */                {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
        {COL_VIO},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_RED},     /* orange → */ {COL_WHT},     /* row 5 */ {COL_WHT},     /* ← orange */ {COL_RED},     {COL_BLK},     {COL_BLK},     {COL_BLK},     {COL_BLK},
                                                                    {COL_BLK},     {COL_BLK},     {COL_BLK},     /* thumb */ {COL_BLK},     {COL_BLK},     {COL_BLK}
    },
};

// Set the layer color from `ledmap`. The ledmap is laid out in
// `LAYOUT_moonlander` argument order; `led_to_layout` translates each
// physical LED index to its slot in that order.
void set_layer_color(int layer) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        uint8_t pos = pgm_read_byte(&led_to_layout[i]);
        rgb_t rgb = {
            .r = pgm_read_byte(&ledmap[layer][pos][0]),
            .g = pgm_read_byte(&ledmap[layer][pos][1]),
            .b = pgm_read_byte(&ledmap[layer][pos][2]),
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
    }
    if (mods & MOD_MASK_CTRL) {
        rgb_matrix_set_color(34, r, g, b);
    }
}

// Handle custom RGB matrix colors.
bool rgb_matrix_indicators_user(void) {
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

#ifdef OS_PASSWORD_BYTES
    static const char os_password[] PROGMEM = { OS_PASSWORD_BYTES, 0x00 };
#else
    #error "OS_PASSWORD_BYTES must be defined in environment variables."
#endif

#ifdef OP_PASSWORD_BYTES
    static const char op_password[] PROGMEM = { OP_PASSWORD_BYTES, 0x00 };
#else
    #error "OP_PASSWORD_BYTES must be defined in environment variables."
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
#define WIN_MNG OSM(MOD_LCTL | MOD_LALT) // Ctrl ⌃ + Opt ⌥
#define BRIGHTU KC_F15
#define BRIGHTD KC_F14
#define LC_TOGG TOGGLE_LAYER_COLOR

// Custom
// ======

enum custom_keycodes {
    MOD_OFF = SAFE_RANGE,
    PW_OS,
    PW_1P,
};

// Handle custom key presses.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    qmk_map_on_key_event(keycode, record);
    switch (keycode) {
        case MOD_OFF:
            if (record->event.pressed) {
                clear_all_mods();
            }
            return false;
        case PW_OS:
            if (record->event.pressed) {
                SEND_STRING(os_password);
                // Press enter to submit the password.
                SEND_STRING(SS_TAP(X_ENTER));
            }
            return false;
        case PW_1P:
            if (record->event.pressed) {
                SEND_STRING(op_password);
                // Press enter to submit the password.
                SEND_STRING(SS_TAP(X_ENTER));
            }
            return false;
    }
    return true;
}

// Handle modifier key releases to ensure pending one-shot mods don't leak.
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case OSM(MOD_LALT):
        case OSM(MOD_LSFT):
        case OSM(MOD_LCTL):
        case OSM(MOD_LGUI): {
            if (!record->event.pressed) {
                // When a held OSM is released, QMK's core has already called
                // unregister_mods() and sent a report. That report carried any
                // pending one-shot mods along to the host. Detect the leak and
                // send a follow-up report without the OSMs, then restore them
                // so they still apply to the next non-mod keypress.
                uint8_t saved_osm = get_oneshot_mods();
                if (saved_osm && (keyboard_report->mods & saved_osm)) {
                    clear_oneshot_mods();
                    send_keyboard_report();
                    set_oneshot_mods(saved_osm);
                }
            }
            break;
        }
    }
    qmk_map_on_mods();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    qmk_map_on_layer(state);
    return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_moonlander(
        KC_GRV,        KC_1,          KC_2,          KC_3,          KC_4,          KC_5,          TD(LAYER_3),   /* row 1 */ XXXXXXX,       KC_6,          KC_7,          KC_8,          KC_9,           KC_0,          KC_MINS,
        KC_TAB,        KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,          MOD_OFF,       /* row 2 */ KC_EQL,        KC_Y,          KC_U,          KC_I,          KC_O,           KC_P,          KC_BSLS,
        CW_TOGG,       KC_A,          KC_S,          KC_D,          KC_F,          KC_G,          KC_ESC,        /* row 3 */ KC_DEL,        KC_H,          KC_J,          KC_K,          KC_L,           KC_SCLN,       KC_QUOT,
        OSM(MOD_LSFT), KC_Z,          KC_X,          KC_C,          KC_V,          KC_B,                         /* row 4 */                KC_N,          KC_M,          KC_COMM,       KC_DOT,         KC_UP,         KC_ENT,
        OSM(MOD_LALT), KC_LBRC,       KC_RBRC,       XXXXXXX,       TD(LAYER_1),   /* orange → */ RAYCAST,       /* row 5 */ WIN_MNG,       /* ← orange */ TD(LAYER_2),   KC_SLSH,       KC_LEFT,        KC_DOWN,       KC_RGHT,
                                                                    KC_SPACE,      OSM(MOD_LGUI), OSM(MOD_LCTL), /* thumb */ OSM(MOD_LALT), OSM(MOD_LSFT), KC_BSPC
    ),
    // Left thumb layer: Contains programming symbols and mouse control.
    [1] = LAYOUT_moonlander(
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       TO(0),         /* row 1 */ XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       KC_UNDS,
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       /* row 2 */ XXXXXXX,       KC_TILD,       KC_PPLS,       KC_PMNS,       KC_PAST,        KC_PSLS,       KC_PIPE,
        XXXXXXX,       TD(PRN),       TD(CBR),       TD(BRC),       TD(ABK),       XXXXXXX,       XXXXXXX,       /* row 3 */ XXXXXXX,       KC_EXLM,       KC_HASH,       KC_UNDS,       KC_AMPR,        KC_COLN,       KC_DQUO,
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,                      /* row 4 */                KC_AT,         KC_DLR,        MS_WHLD,       MS_BTN1,        MS_UP,         MS_BTN2,
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       TO(0),         /* orange → */ PW_OS,         /* row 5 */ PW_1P,         /* ← orange */ TO(0),         MS_WHLU,       MS_LEFT,        MS_DOWN,       MS_RGHT,
                                                                    _______,       _______,       _______,       /* thumb */ _______,       _______,       _______
    ),
    // Right thumb layer: Contains volume, brightness and media controls. Also controls desk height via Raycast key-combos.
    [2] = LAYOUT_moonlander(
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       TO(0),         /* row 1 */ XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       KC_VOLU,       KC_MPRV,       BRIGHTU,       /* row 2 */ XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       KC_VOLD,       KC_MNXT,       BRIGHTU,       /* row 3 */ XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       KC_MUTE,       KC_MPLY,                      /* row 4 */                XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
        XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       TO(0),         /* orange → */ LCAG(KC_UP),   /* row 5 */ LCAG(KC_DOWN), /* ← orange */ TO(0),         XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
                                                                    _______,       _______,       _______,       /* thumb */ _______,       _______,       _______
    ),
    // Contains controls for this keyboard.
    [3] = LAYOUT_moonlander(
        AU_TOGG,       MU_TOGG,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       TO(0),         /* row 1 */ XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       QK_BOOT,
        LC_TOGG,       XXXXXXX,       RM_SPDU,       RM_HUEU,       RM_SATU,       RM_VALU,       XXXXXXX,       /* row 2 */ XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
        RM_TOGG,       XXXXXXX,       RM_SPDD,       RM_HUED,       RM_SATD,       RM_VALD,       XXXXXXX,       /* row 3 */ XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
        RM_PREV,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,                      /* row 4 */                XXXXXXX,       XXXXXXX,       XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
        RM_NEXT,       XXXXXXX,       XXXXXXX,       XXXXXXX,       TO(0),         /* orange → */ LCAG(KC_UP),   /* row 5 */ LCAG(KC_DOWN), /* ← orange */ TO(0),         XXXXXXX,       XXXXXXX,        XXXXXXX,       XXXXXXX,
                                                                    _______,       _______,       _______,       /* thumb */ _______,       _______,       _______
    ),
};
