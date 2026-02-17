/*
 * FamiKey Default Keymap
 *
 * This keymap mirrors the original Arduino implementation,
 * mapping Famicom keyboard keys to standard USB HID keys.
 *
 * Matrix layout: 18 columns × 4 rows
 * Each column has 4 keys (rows 0-3)
 */

#include QMK_KEYBOARD_H

/* Custom keycode for KANA key (not in standard QMK) */
#define KC_KANA 0x88

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        // COL 00 (rows 0-3) - swapped rows 1&2
        KC_F8,      KC_LBRC,    KC_ENT,     KC_RBRC,

        // COL 01 (rows 0-3) - swapped rows 1&2
        KC_KANA,    KC_BSLS,    KC_RSFT,    KC_BSPC,

        // COL 02 (rows 0-3) - swapped rows 1&2
        KC_F7,      KC_QUOT,    KC_GRV,     KC_SCLN,

        // COL 03 (rows 0-3) - swapped rows 1&2
        KC_RALT,    KC_MINS,    KC_SLSH,    KC_EQL,

        // COL 04 (rows 0-3) - swapped rows 1&2
        KC_F6,      KC_L,       KC_O,       KC_K,

        // COL 05 (rows 0-3) - swapped rows 1&2
        KC_DOT,     KC_P,       KC_COMM,    KC_0,

        // COL 06 (rows 0-3) - swapped rows 1&2
        KC_F5,      KC_U,       KC_I,       KC_J,

        // COL 07 (rows 0-3) - swapped rows 1&2
        KC_M,       KC_9,       KC_N,       KC_8,

        // COL 08 (rows 0-3) - swapped rows 1&2
        KC_F4,      KC_G,       KC_Y,       KC_H,

        // COL 09 (rows 0-3) - swapped rows 1&2
        KC_B,       KC_7,       KC_V,       KC_6,

        // COL 10 (rows 0-3) - swapped rows 1&2
        KC_F3,      KC_R,       KC_T,       KC_D,

        // COL 11 (rows 0-3) - swapped rows 1&2
        KC_F,       KC_5,       KC_C,       KC_4,

        // COL 12 (rows 0-3) - swapped rows 1&2
        KC_F2,      KC_S,       KC_W,       KC_A,

        // COL 13 (rows 0-3) - swapped rows 1&2
        KC_X,       KC_E,       KC_Z,       KC_3,

        // COL 14 (rows 0-3) - swapped rows 1&2
        KC_F1,      KC_Q,       KC_ESC,     KC_LCTL,

        // COL 15 (rows 0-3) - swapped rows 1&2
        KC_LSFT,    KC_1,       KC_LGUI,    KC_2,

        // COL 16 (rows 0-3) - swapped rows 1&2
        KC_HOME,    KC_RGHT,    KC_UP,      KC_LEFT,

        // COL 17 (rows 0-3) - swapped rows 1&2
        KC_DOWN,    KC_DEL,     KC_SPC,     KC_INS
    )
};

/*
 * Notes on key mappings:
 *
 * - KC_KANA: Japanese KANA key (0x88)
 * - KC_LGUI: Maps to GRPH key (Windows/Command key)
 * - KC_BSPC: Maps to STOP key
 * - KC_RALT: Maps to underscore key
 *
 * Physical layout on Famicom keyboard differs from standard keyboards.
 * This mapping preserves the original key positions and functions.
 */

// Auto-enable debug on startup (override: set to false to disable)
#ifndef AUTO_ENABLE_DEBUG
#define AUTO_ENABLE_DEBUG true
#endif

void keyboard_post_init_user(void) {
    // Enable debug automatically on startup
    // Comment out these lines if you want debug off by default
    if (AUTO_ENABLE_DEBUG) {
        debug_enable = true;
        debug_matrix = true;
        debug_keyboard = true;
        uprintf("Debug auto-enabled on startup (press F8 to toggle)\n");
    }
}

// Track KANA keycode for bootloader combo
static bool kana_keycode_pressed = false;

// Toggle debug mode with F8, enter bootloader with KANA+F8 (by keycode)
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Track KC_KANA keycode regardless of which physical key it's mapped to
    if (keycode == KC_KANA) {
        kana_keycode_pressed = record->event.pressed;
    }

    // KC_F8 keycode: toggle debug or enter bootloader if KC_KANA is held
    if (keycode == KC_F8 && record->event.pressed) {
        if (kana_keycode_pressed) {
            // KC_KANA + KC_F8: Enter bootloader
            uprintf("Entering bootloader mode...\n");
            reset_keyboard();
            return false;
        } else {
            // KC_F8 alone: Toggle debug
            debug_enable = !debug_enable;
            debug_matrix = debug_enable;
            debug_keyboard = debug_enable;
            if (debug_enable) {
                uprintf("Debug ENABLED\n");
            } else {
                uprintf("Debug DISABLED\n");
            }
        }
    }
    return true;
}
