// firmware/supreme/qmk_map.c
//
// Raw HID protocol implementation for qmk_map.

#include "qmk_map.h"

#ifdef RAW_ENABLE

#include "raw_hid.h"
#include <string.h>

// Declared in keymap.c — qmk_map.c needs to read it across compilation units.
extern const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS];

#define QMK_MAP_PROTO_VER 1
#define RAW_EPSIZE 32

// Tags — must match qmk_map/protocol.py.
enum {
    CMD_HELLO        = 0x01,
    CMD_PING         = 0x02,
    RSP_HELLO_ACK    = 0x81,
    RSP_KEYMAP_CHUNK = 0x82,
    RSP_LAYER        = 0x83,
    RSP_KEY_EVENT    = 0x84,
    RSP_MODS         = 0x85,
    RSP_PONG         = 0x86,
    RSP_CAPS_WORD    = 0x87,
};

#define LAYER_COUNT     4
#define KEYS_PER_LAYER  72
#define ENTRIES_PER_CHUNK 7   // 32B - 3B header = 29B; 4B/entry → 7

// matrix (row,col) → key_index in LAYOUT_moonlander argument order.
// Derived from keyboards/zsa/moonlander/reva/keyboard.json (LAYOUT entry).
//
// MATRIX_ROWS = 12, MATRIX_COLS = 7.
// Left half is rows 0..5; right half is rows 6..11.
// idx 59 (left "orange" key — RAYCAST in supreme) lives on row 5 with the left thumbs.
// idx 60 (right "orange" key — WIN_MGMT in supreme) lives on row 11 with the right thumbs.
// Entries set to 0xff are matrix positions the macro skips.
static const uint8_t matrix_to_index[MATRIX_ROWS][MATRIX_COLS] = {
    // Left half
    {  0,  1,  2,  3,  4,  5,  6 },     // row 0 — top
    { 14, 15, 16, 17, 18, 19, 20 },     // row 1
    { 28, 29, 30, 31, 32, 33, 34 },     // row 2
    { 42, 43, 44, 45, 46, 47, 0xff },   // row 3 (no col 6)
    { 54, 55, 56, 57, 58, 0xff, 0xff }, // row 4 (cols 0..4)
    { 66, 67, 68, 59, 0xff, 0xff, 0xff },// row 5 — thumbs (66, 67, 68) + orange RAYCAST (59)
    // Right half
    {  7,  8,  9, 10, 11, 12, 13 },     // row 6 — top
    { 21, 22, 23, 24, 25, 26, 27 },     // row 7
    { 35, 36, 37, 38, 39, 40, 41 },     // row 8
    { 0xff, 48, 49, 50, 51, 52, 53 },   // row 9 (cols 1..6)
    { 0xff, 0xff, 61, 62, 63, 64, 65 }, // row 10 (cols 2..6)
    { 0xff, 0xff, 0xff, 60, 69, 70, 71 },// row 11 — orange WIN_MGMT (60) + thumbs (69, 70, 71)
};

static uint8_t last_mods = 0xff;  // force a push on first call
static int8_t  last_caps_word = -1;  // force a push on first call

static void send_buf(const uint8_t *buf) {
    raw_hid_send((uint8_t *)buf, RAW_EPSIZE);
}

static void push_layer(uint8_t layer) {
    uint8_t buf[RAW_EPSIZE] = {0};
    buf[0] = RSP_LAYER;
    buf[1] = layer;
    send_buf(buf);
}

static void push_key_event(uint8_t key_index, bool pressed, uint8_t layer) {
    if (key_index == 0xff) return;
    uint8_t buf[RAW_EPSIZE] = {0};
    buf[0] = RSP_KEY_EVENT;
    buf[1] = key_index;
    buf[2] = pressed ? 1 : 0;
    buf[3] = layer;
    send_buf(buf);
}

static void push_mods(uint8_t mods) {
    uint8_t buf[RAW_EPSIZE] = {0};
    buf[0] = RSP_MODS;
    buf[1] = mods;
    send_buf(buf);
}

static void push_caps_word(bool on) {
    uint8_t buf[RAW_EPSIZE] = {0};
    buf[0] = RSP_CAPS_WORD;
    buf[1] = on ? 1 : 0;
    send_buf(buf);
}

static void send_hello_ack(void) {
    uint8_t buf[RAW_EPSIZE] = {0};
    buf[0] = RSP_HELLO_ACK;
    buf[1] = QMK_MAP_PROTO_VER;
    buf[2] = LAYER_COUNT;
    buf[3] = (uint8_t)(KEYS_PER_LAYER & 0xff);
    buf[4] = (uint8_t)(KEYS_PER_LAYER >> 8);
    send_buf(buf);
}

static uint8_t total_chunks(void) {
    uint16_t total_entries = (uint16_t)LAYER_COUNT * (uint16_t)KEYS_PER_LAYER;
    return (uint8_t)((total_entries + ENTRIES_PER_CHUNK - 1) / ENTRIES_PER_CHUNK);
}

static void send_snapshot(void) {
    send_hello_ack();

    uint8_t buf[RAW_EPSIZE];
    const uint8_t chunks = total_chunks();
    uint8_t chunk_idx = 0;
    uint8_t entries_in_chunk = 0;
    memset(buf, 0, sizeof(buf));
    buf[0] = RSP_KEYMAP_CHUNK;
    buf[1] = chunk_idx;
    buf[2] = chunks;
    uint8_t *p = buf + 3;

    for (uint8_t layer = 0; layer < LAYER_COUNT; layer++) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uint8_t idx = matrix_to_index[row][col];
                if (idx == 0xff) continue;
                uint16_t kc = pgm_read_word(&keymaps[layer][row][col]);
                *p++ = layer;
                *p++ = idx;
                *p++ = (uint8_t)(kc & 0xff);
                *p++ = (uint8_t)(kc >> 8);
                entries_in_chunk++;

                if (entries_in_chunk == ENTRIES_PER_CHUNK) {
                    send_buf(buf);
                    chunk_idx++;
                    memset(buf, 0, sizeof(buf));
                    buf[0] = RSP_KEYMAP_CHUNK;
                    buf[1] = chunk_idx;
                    buf[2] = chunks;
                    p = buf + 3;
                    entries_in_chunk = 0;
                }
            }
        }
    }
    if (entries_in_chunk > 0) {
        send_buf(buf);
    }

    push_layer(get_highest_layer(layer_state));
    push_mods(get_mods() | get_oneshot_mods());
    last_mods = get_mods() | get_oneshot_mods();
    push_caps_word(is_caps_word_on());
    last_caps_word = is_caps_word_on() ? 1 : 0;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length < 1) return;
    switch (data[0]) {
        case CMD_HELLO:
            send_snapshot();
            break;
        case CMD_PING: {
            uint8_t buf[RAW_EPSIZE] = {0};
            buf[0] = RSP_PONG;
            send_buf(buf);
            break;
        }
        default:
            break;
    }
}

// Public callbacks used by keymap.c
void qmk_map_on_layer(layer_state_t state) {
    push_layer(get_highest_layer(state));
}

void qmk_map_on_key_event(uint16_t keycode, keyrecord_t *record) {
    (void)keycode;
    if (record == NULL) return;
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return;
    uint8_t idx = matrix_to_index[row][col];
    push_key_event(idx, record->event.pressed, get_highest_layer(layer_state));
}

void qmk_map_on_mods(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    if (mods != last_mods) {
        last_mods = mods;
        push_mods(mods);
    }
}

void qmk_map_on_caps_word(void) {
    int8_t cur = is_caps_word_on() ? 1 : 0;
    if (cur != last_caps_word) {
        last_caps_word = cur;
        push_caps_word(cur != 0);
    }
}

// Polled every matrix scan. Catches mod-state changes that bypass
// post_process_record_user — notably keys whose process_record_user returns
// false (e.g. MOD_OFF in supreme calls clear_all_mods then `return false`,
// so post_process_record_user is never invoked). Also catches caps-word
// transitions since QMK toggles it inside its own internal hook.
void housekeeping_task_user(void) {
    qmk_map_on_mods();
    qmk_map_on_caps_word();
}

#endif  // RAW_ENABLE
