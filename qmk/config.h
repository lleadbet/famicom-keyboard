/*
 * FamiKey - Famicom Keyboard USB Adapter
 * QMK Configuration
 */

#pragma once

/* USB Device descriptor - now configured in keyboard.json */

/* Matrix configuration */
#define MATRIX_ROWS 18
#define MATRIX_COLS 4

/* Custom matrix implementation */
#define CUSTOM_MATRIX

/* Debounce reduces chatter (unintended double-presses) */
/* Our custom matrix does its own debouncing, so set this to 0 */
#define DEBOUNCE 0

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* Key matrix is scanned every 1ms by default, which is fine for this keyboard */
/* Uncomment and adjust if needed:
 * #define MATRIX_SCAN_RATE 1000
 */
