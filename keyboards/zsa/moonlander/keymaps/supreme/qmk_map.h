// firmware/supreme/qmk_map.h
//
// Drop into qmk_firmware/keyboards/zsa/moonlander/keymaps/supreme/.
// Public callbacks called from keymap.c. All no-ops when RAW_ENABLE is off.

#pragma once

#include "quantum.h"

#ifdef RAW_ENABLE
void qmk_map_on_layer(layer_state_t state);
void qmk_map_on_key_event(uint16_t keycode, keyrecord_t *record);
void qmk_map_on_mods(void);
#else
static inline void qmk_map_on_layer(layer_state_t state) { (void)state; }
static inline void qmk_map_on_key_event(uint16_t keycode, keyrecord_t *record) {
    (void)keycode; (void)record;
}
static inline void qmk_map_on_mods(void) {}
#endif
