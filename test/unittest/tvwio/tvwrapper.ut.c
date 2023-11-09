/* Unit tests for tvwrapper.c.
 * AGPL-3.0 license
 * Initial rev 2023-11-08 Misha Turnbull
 *
 * This file implements unit tests for the tvwrapper.c sourcecode.
 */

#include <signal.h>

#include "../unittest.h"
#include "tvwio/tvwrapper.h"

/**
 * Holds test data for tvwrapper freeing function tests.
 */
struct tvwrap {
    struct WRAPPER_FILE *wrap;
} tvwrap;

/**
 * Setup function for tvwrapper freeing function tests.
 *
 * Called before every test in this test suite.
 */
static void setup(void) {
    tvwrap.wrap = (struct WRAPPER_FILE*)calloc(1, sizeof(struct WRAPPER_FILE));
}

/**
 * Teardown function for tvwrapper freeing function tests.
 *
 * Called after every test in this test suite.
 */
static void teardown(void) {
    tvwfree(tvwrap.wrap);
}

/**
 * Declare test suite for _tvwfreeparts function.
 */
TestSuite(_tvwfreeparts,
        .init = setup,
        .fini = teardown);

/**
 * Test the file-pointer closing in _tvwfreeparts.
 */
Test(_tvwfreeparts, fileptr) {
    tvwrap.wrap->fp = fopen(CONFIG_SINKFILE, "r");
    
    _tvwfreeparts(tvwrap.wrap);

    cr_expect(eq(ptr, tvwrap.wrap->fp, NULL),
            "_tvwfreeparts did not close file pointer!");
}

/**
 * Test the freeing of filename strings in _tvwfreeparts.
 */
Test(_tvwfreeparts, filename) {
    tvwrap.wrap->filename = calloc(14, sizeof(char));
    strncpy(tvwrap.wrap->filename, "Hello, world!", 14);

    _tvwfreeparts(tvwrap.wrap);

    cr_expect(eq(ptr, tvwrap.wrap->filename, NULL),
            "_tvwfreeparts did not free filename string!");
}

/**
 * Test the freeing of metadata strings in _tvwfreeparts.
 */
Test(_tvwfreeparts, metadata) {
    tvwrap.wrap->metadata = calloc(256, sizeof(char));
    // only up to 255 -- leaves the null terminator
    for (int i = 0; i < 255; i++) {
        tvwrap.wrap->metadata[i] = 'A';
    }

    _tvwfreeparts(tvwrap.wrap);

    cr_expect(eq(ptr, tvwrap.wrap->metadata, NULL),
            "_tvwfreeparts did not free metadata string!");
}

/**
 * Test the freeing of file content strings in _tvwfreeparts.
 */
Test(_tvwfreeparts, contents) {
    tvwrap.wrap->contents = calloc(256, sizeof(char));
    // only up to 255 -- leaves the null terminator
    for (int i = 0; i < 255; i++) {
        tvwrap.wrap->contents[i] = 'A';
    }

    _tvwfreeparts(tvwrap.wrap);

    cr_expect(eq(ptr, tvwrap.wrap->contents, NULL),
            "_tvwfreeparts did not free contents string!");
}

