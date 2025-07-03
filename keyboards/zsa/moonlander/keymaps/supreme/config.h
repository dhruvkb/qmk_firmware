#pragma once

// ZSA configuration
#define SERIAL_NUMBER "PAelL/latest" // used by apps like Keymapp and typ.ing

// Limit layer count: https://docs.qmk.fm/squeezing_avr#layers
#define LAYER_STATE_8BIT

// RGB Matrix: https://docs.qmk.fm/features/rgb_matrix
#define RGB_MATRIX_DEFAULT_SPD 60

// Auto Shift: https://docs.qmk.fm/features/auto_shift
#define AUTO_SHIFT_TIMEOUT 200 // ms
#define NO_AUTO_SHIFT_TAB // prevents reverse focus order

// Caps Word: https://docs.qmk.fm/features/caps_word
#define CAPS_WORD_TIMEOUT 3000 // ms

// Audio: https://docs.qmk.fm/features/audio
#define STARTUP_SONG SONG(STARTUP_SOUND)
#define GOODBYE_SONG SONG(GOODBYE_SOUND)

// Unwanted animations: https://docs.qmk.fm/squeezing_avr#rgb-settings
#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#undef ENABLE_RGB_MATRIX_RAINDROPS
#undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#undef ENABLE_RGB_MATRIX_HUE_PENDULUM
#undef ENABLE_RGB_MATRIX_HUE_WAVE
#undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#undef ENABLE_RGB_MATRIX_PIXEL_FLOW
#undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#undef ENABLE_RGB_MATRIX_SPLASH
#undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#undef ENABLE_RGB_MATRIX_STARLIGHT
#undef ENABLE_RGB_MATRIX_STARLIGHT_DUAL_HUE
#undef ENABLE_RGB_MATRIX_STARLIGHT_DUAL_SAT
