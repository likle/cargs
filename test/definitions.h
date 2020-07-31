#pragma once

#include "tests.h"

/**
 * This is just a small macro which calculates the size of an array.
 */
#define CAG_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * Creates prototypes for all test functions.
 */
#define XX(u, t) int u##_##t(void);
UNIT_TESTS(XX)
#undef XX
