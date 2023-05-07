#pragma once

#include "tests.h"
/**
 * Creates prototypes for all test functions.
 */
#define XX(u, t) int u##_##t(void);
UNIT_TESTS(XX)
#undef XX
