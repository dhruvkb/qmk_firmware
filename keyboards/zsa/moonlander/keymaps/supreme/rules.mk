# Raw HID for qmk_map TUI.
RAW_ENABLE = yes
SRC += qmk_map.c

# Auto Shift: https://docs.qmk.fm/features/auto_shift
AUTO_SHIFT_ENABLE = yes

# Tap Dance: https://docs.qmk.fm/features/tap_dance
TAP_DANCE_ENABLE = yes

# Caps Word: https://docs.qmk.fm/features/caps_word
CAPS_WORD_ENABLE = yes

# Passwords

# Abort immediately if OS_PASSWORD_BYTES or OP_PASSWORD_BYTES weren’t provided.
ifndef OS_PASSWORD_BYTES
  $(error OS_PASSWORD_BYTES must be defined in environment variables.)
endif
ifndef OP_PASSWORD_BYTES
  $(error OP_PASSWORD_BYTES must be defined in environment variables.)
endif

# Inject the bytes as C pre-processor definitions. The values are comma-
# separated hex byte literals (e.g. 0x62,0x42,...) so they survive shell
# quoting unscathed regardless of what characters appear in the password.
EXTRAFLAGS += -DOS_PASSWORD_BYTES=$(OS_PASSWORD_BYTES)
EXTRAFLAGS += -DOP_PASSWORD_BYTES=$(OP_PASSWORD_BYTES)
