# Auto Shift: https://docs.qmk.fm/features/auto_shift
AUTO_SHIFT_ENABLE = yes

# Tap Dance: https://docs.qmk.fm/features/tap_dance
TAP_DANCE_ENABLE = yes

# Caps Word: https://docs.qmk.fm/features/caps_word
CAPS_WORD_ENABLE = yes

# Passwords

# Abort immediately if OS_PASSWORD or OP_PASSWORD weren’t provided.
ifndef OS_PASSWORD
  $(error OS_PASSWORD must be defined in environment variables.)
endif
ifndef OP_PASSWORD
  $(error OP_PASSWORD must be defined in environment variables.)
endif

# Inject the text as a C pre-processor definition
# Escape any pipe characters in OS_PASSWORD for the preprocessor definition.
OS_PASSWORD_ESCAPED := $(subst |,\|,$(OS_PASSWORD))
EXTRAFLAGS += -DOS_PASSWORD=\"$(OS_PASSWORD_ESCAPED)\"
# Escape any pipe characters in OP_PASSWORD for the preprocessor definition.
OP_PASSWORD_ESCAPED := $(subst |,\|,$(OP_PASSWORD))
EXTRAFLAGS += -DOP_PASSWORD=\"$(OP_PASSWORD_ESCAPED)\"
