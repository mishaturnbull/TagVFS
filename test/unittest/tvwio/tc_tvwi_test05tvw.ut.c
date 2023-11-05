/* tvw_i test case for test05.tvw
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-11-04
 *
 * Tests the tvw_i section on the test05.tvw test case/
 */

#include "../unittest.h"
#include "tvwio/tvw_i.h"

/**
 * Holds test data for case test05.tvw.
 *
 * Used to pass data from setup functions, to the test case, and then to the
 * teardown function.
 */
struct test05 {
    struct WRAPPER_FILE *file;
    int err;
} test05;

/**
 * Pre-test setup function for test05.tvw test case.
 *
 * Called before every test case to ensure they start from the same point.
 */
static void setup(void) {
    test05.file = (struct WRAPPER_FILE*)calloc(
            1, sizeof(struct WRAPPER_FILE));
    test05.err = read_wrapper("test/resources/test05.tvw", test05.file);
    cr_assert(test05.err == 0,
            "got error code %d from read_wrapper!", test05.err);
}

/**
 * Post-test teardown function for test05.tvw test case.
 *
 * Called after every test case to ensure they end cleanly.
 */
static void teardown(void) {
    tvwfree(test05.file);
}

/**
 * Declare test case for test05.tvw, using the defined setup and teardown
 * functions.
 */
TestSuite(tc_tvwi_test05tvw,
        .init = setup,
        .fini = teardown);

/**
 * Check disk IO related functionality.
 */
Test(tc_tvwi_test05tvw, fileops) {
    cr_expect(eq(str, test05.file->filename, "test/resources/test05.tvw"),
            "wrapper file has wrong filename!");
    cr_expect(eq(ptr, test05.file->fp, NULL),
            "wrapper file pointer is still open!");
}

/**
 * Check the header data was read correctly, against known-good values.
 */
Test(tc_tvwi_test05tvw, header) {
    cr_expect(eq(u32, test05.file->header.format_version, 0),
            "wrong format version!");
    cr_expect(eq(u64, test05.file->header.start_of_contents_gs, 196),
            "wrong SOC GS offset!");
    cr_expect(eq(u64, test05.file->header.len_of_contents, 48),
            "wrong contents length!");
    cr_expect(eq(u16, test05.file->header.comp_algo_meta, 0),
            "wrong metadata compression algo!");
    cr_expect(eq(u16, test05.file->header.comp_algo_file, 0),
        "wrong contents compression algo!");
    uint8_t good[] = {
        0xa6, 0xe3, 0x84, 0x34, 0x4f, 0x60, 0x61, 0x23,
        0x5e, 0x5a, 0xc5, 0xa6, 0x9e, 0xa2, 0xb4, 0x3c,
        0x5b, 0x07, 0x5e, 0xd2, 0x74, 0xb8, 0x79, 0x68,
        0x4c, 0xf5, 0xe2, 0x9f, 0x3e, 0x05, 0x60, 0x35,
        0x9e, 0x4f, 0xd2, 0x1a, 0xd1, 0x74, 0xd1, 0xc9,
        0xba, 0xb4, 0xa4, 0xcb, 0xf9, 0x4e, 0x9b, 0xd1,
        0xc9, 0x38, 0x55, 0xbd, 0xe6, 0xb4, 0x55, 0x79,
        0x51, 0x5e, 0x9b, 0xbd, 0xd9, 0xa8, 0x0b, 0xc5
    };
    cr_expect(eq(u8[64], test05.file->header.sha512, good),
            "wrong (stored) file contents hash!");
}

/**
 * Check the test05.tvw metadata was read correctly, against known-good values.
 */
Test(tc_tvwi_test05tvw, metadata) {
    cr_expect(ne(ptr, test05.file->xmlerr, NULL),
            "xmlerr is NULL, parsing succeeded??");
    cr_assert(eq(ptr, test05.file->xmlroot, NULL),
            "xmlroot is not NULL, parsing succeeded??");
    cr_expect(eq(sz, test05.file->sizeof_meta, 108),
            "did not read correct metadata size!");
    cr_assert(ne(ptr, test05.file->metadata, NULL),
            "metadata is NULL; something went wrong!");

    char good[] = \
"This file's length_of_contents has been altered to indicate *less* contents"\
" present than there actually are.";

    cr_assert(eq(str, test05.file->metadata, good),
            "(raw) metadata is incorrect!");
}

/**
 * Check the test05.tvw file contents were read correctly, against known-good
 * values.
 */
Test(tc_tvwi_test05tvw, contents) {
    cr_expect(ne(ptr, test05.file->contents, NULL),
            "file contents is NULL, something failed!");
    cr_expect(eq(sz, test05.file->sizeof_cont, 48),
            "contents size is incorrect!");

    char good[] = \
"Hello, world!\n"\
"Hello, world! (x2)\n"\
"Hello, world! (";

    cr_expect(eq(str, test05.file->contents, good),
            "contents are incorrect!");
}

