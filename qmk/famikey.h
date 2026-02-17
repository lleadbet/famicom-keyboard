/*
 * FamiKey keyboard header
 */

#pragma once

#include "quantum.h"

/*
 * Matrix layout for Famicom keyboard
 *
 * The matrix is 18 columns × 4 rows, scanned sequentially.
 * This layout macro maps physical key positions to the matrix.
 *
 * Arguments go from k00 to k17_3 representing column_row positions.
 */

#define LAYOUT( \
    k00, k01, k02, k03, k10, k11, k12, k13, k20, k21, k22, k23, k30, k31, k32, k33, k40, k41, k42, k43, \
    k50, k51, k52, k53, k60, k61, k62, k63, k70, k71, k72, k73, k80, k81, k82, k83, k90, k91, k92, k93, \
    k100, k101, k102, k103, k110, k111, k112, k113, k120, k121, k122, k123, k130, k131, k132, k133, k140, k141, k142, k143, \
    k150, k151, k152, k153, k160, k161, k162, k163, k170, k171, k172, k173 \
) { \
    { k00, k01, k02, k03 }, \
    { k10, k11, k12, k13 }, \
    { k20, k21, k22, k23 }, \
    { k30, k31, k32, k33 }, \
    { k40, k41, k42, k43 }, \
    { k50, k51, k52, k53 }, \
    { k60, k61, k62, k63 }, \
    { k70, k71, k72, k73 }, \
    { k80, k81, k82, k83 }, \
    { k90, k91, k92, k93 }, \
    { k100, k101, k102, k103 }, \
    { k110, k111, k112, k113 }, \
    { k120, k121, k122, k123 }, \
    { k130, k131, k132, k133 }, \
    { k140, k141, k142, k143 }, \
    { k150, k151, k152, k153 }, \
    { k160, k161, k162, k163 }, \
    { k170, k171, k172, k173 } \
}
