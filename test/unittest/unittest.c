#include "unittest.h"

/**
 * Convert a constant string to a cr_malloc() string.
 *
 * Necessary because Criterion needs to control memory allocation during
 * parameterized test runs -- see
 * https://criterion.readthedocs.io/en/latest/parameterized.html#dynamically-allocating-parameters.
 */
char *crs(const char *str, size_t sz) {
    char *out = cr_malloc(sz + 1);
    memcpy(out, str, sz);
    return out;
}
