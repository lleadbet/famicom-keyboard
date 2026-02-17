/*
 * Custom matrix driver for Famicom Keyboard
 *
 * The Famicom keyboard uses an external scanning protocol with:
 * - 3 control pins (Reset, Column Select, Enable)
 * - 4 data pins (reading 4 rows simultaneously)
 * - 18 columns × 4 rows = 72 keys total
 *
 * Based on original Arduino implementation by Circuit Rewind
 */

#include "quantum.h"
#include "matrix.h"
#include "wait.h"
#include "print.h"

/* Pin Definitions */
#define KB_RESET    GP2   // Physical Pin D2
#define KB_COL_SEL  GP3   // Physical Pin D3
#define KB_ENABLE   GP4   // Physical Pin D4

#define KB_C2_D1    GP26  // Physical Pin A0
#define KB_C2_D2    GP27  // Physical Pin A1
#define KB_C2_D3    GP28  // Physical Pin A2
#define KB_C2_D4    GP29  // Physical Pin A3


/* Matrix dimensions */
#define MATRIX_ROWS 18
#define MATRIX_COLS 4

/* Debounce counter threshold */
#define DEBOUNCE_THRESHOLD 8

/* State tracking */
static matrix_row_t raw_matrix[MATRIX_ROWS];
static matrix_row_t matrix[MATRIX_ROWS];
static uint8_t debounce_counters[MATRIX_ROWS][MATRIX_COLS];

/*
 * Slow down clock pulses to not overwhelm the vintage keyboard hardware
 * NOTE: Lower delay values cause frequent read errors from the keyboard
 */
static inline void delayed_write_pin(pin_t pin, uint8_t level) {
    writePin(pin, level);
    wait_us(50);  // 50 microsecond delay for vintage hardware
}

/*
 * Read the current state of all 4 rows from the data pins
 * Returns a 4-bit value representing the row state
 */
static inline matrix_row_t read_rows(void) {
    return (readPin(KB_C2_D1) << 0)
         | (readPin(KB_C2_D2) << 1)
         | (readPin(KB_C2_D3) << 2)
         | (readPin(KB_C2_D4) << 3);
}

void matrix_init_custom(void) {
    /* Initialize all state arrays */
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        raw_matrix[i] = 0;
        matrix[i] = 0;
        for (uint8_t j = 0; j < MATRIX_COLS; j++) {
            debounce_counters[i][j] = 0;
        }
    }

    /* Configure data input pins with pulldown to prevent floating when unplugged */
    setPinInputLow(KB_C2_D1);
    setPinInputLow(KB_C2_D2);
    setPinInputLow(KB_C2_D3);
    setPinInputLow(KB_C2_D4);

    /* Configure control output pins */
    setPinOutput(KB_RESET);
    setPinOutput(KB_COL_SEL);
    setPinOutput(KB_ENABLE);

    /* Initialize keyboard to known state */
    delayed_write_pin(KB_RESET, 1);
    delayed_write_pin(KB_RESET, 0);
    delayed_write_pin(KB_COL_SEL, 0);
    delayed_write_pin(KB_ENABLE, 1);

    uprintf("FamiKey matrix initialized\n");
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;
    uint8_t col_toggle = 0;

    /* Scan all 18 columns */
    for (uint8_t col = 0; col < MATRIX_ROWS; col++) {
        /* Read current row state (4 bits) */
        matrix_row_t new_state = read_rows();

        /* Debug: Log non-zero column states (only when keys are pressed) */
        if (debug_matrix && new_state != 0) {
            uprintf("Col %2d: 0x%01X (", col, new_state);
            for (int i = 3; i >= 0; i--) {
                uprintf("%d", (new_state >> i) & 1);
            }
            uprintf(")\n");
        }

        /* Process each of the 4 rows in this column */
        for (uint8_t row = 0; row < MATRIX_COLS; row++) {
            uint8_t new_bit = (new_state >> row) & 0x01;
            uint8_t old_bit = (raw_matrix[col] >> row) & 0x01;

            /* Key state is unchanged, reset debounce counter */
            if (new_bit == old_bit) {
                debounce_counters[col][row] = 0;
            }
            /* Key state has changed */
            else {
                debounce_counters[col][row]++;

                /* State has been consistent for DEBOUNCE_THRESHOLD scans */
                if (debounce_counters[col][row] >= DEBOUNCE_THRESHOLD) {
                    debounce_counters[col][row] = 0;

                    /* Update the raw matrix state */
                    if (new_bit) {
                        raw_matrix[col] |= (1 << row);
                        if (debug_enable) {
                            uprintf("Key PRESS: [%2d,%d]\n", col, row);
                        }
                    } else {
                        raw_matrix[col] &= ~(1 << row);
                        if (debug_enable) {
                            uprintf("Key RELEASE: [%2d,%d]\n", col, row);
                        }
                    }

                    changed = true;
                }
            }
        }

        /* Toggle column select pulse line to advance to next column */
        delayed_write_pin(KB_COL_SEL, (++col_toggle & 0x01));
    }

    /* Reset column counter */
    delayed_write_pin(KB_COL_SEL, 0);

    /* Reset row counter */
    delayed_write_pin(KB_RESET, 1);
    delayed_write_pin(KB_RESET, 0);

    /* Copy debounced state to output matrix if anything changed */
    if (changed) {
        for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
            current_matrix[i] = raw_matrix[i];
        }
    }

    return changed;
}
