# Meaning of Just prefixes:
# @ - Quiet recipes (https://github.com/casey/just#quiet-recipes)
# _ - Private recipes (https://github.com/casey/just#private-recipes)

set dotenv-load := false

# Show all available recipes
@_default:
	just --list --unsorted

# Compile the QMK firmware, providing the encoded passwords.
compile:
	#!/usr/bin/env bash
	to_hex() { printf '%s' "$1" | od -An -tx1 -v | tr -d ' \n' | sed 's/../0x&,/g; s/,$//'; }
	export OS_PASSWORD_BYTES="$(to_hex "$(op read op://Personal/qf5mtzhha4spdyey3xwckfwom4/password)")"
	export OP_PASSWORD_BYTES="$(to_hex "$(op read op://Personal/1Password/password)")"
	qmk compile

# Flash the precompiled firmware to the keyboard.
flash:
	qmk flash zsa_moonlander_reva_supreme.bin
