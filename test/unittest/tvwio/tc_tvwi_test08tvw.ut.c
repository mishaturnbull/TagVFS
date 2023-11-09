/* tvw_i test case for test08.tvw
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-11-04
 *
 * Tests the tvw_i section on the test08.tvw test case
 */

#include "../unittest.h"
#include "tvwio/tvw_i.h"

/**
 * Holds test data for case test08.tvw.
 *
 * Used to pass data from setup functions, to the test case, and then to the
 * teardown function.
 */
struct test08 {
    struct WRAPPER_FILE *file;
    int err;
} test08;

/**
 * Pre-test setup function for test08.tvw test case.
 *
 * Called before every test case to ensure they start from the same point.
 */
static void setup(void) {
    test08.file = (struct WRAPPER_FILE*)calloc(
            1, sizeof(struct WRAPPER_FILE));
    test08.err = read_wrapper("test/resources/test08.tvw", test08.file);
    cr_assert(test08.err == 0,
            "got error code %d from read_wrapper!", test08.err);
}

/**
 * Post-test teardown function for test08.tvw test case.
 *
 * Called after every test case to ensure they end cleanly.
 */
static void teardown(void) {
    tvwfree(test08.file);
}

/**
 * Declare test case for test08.tvw, using the defined setup and teardown
 * functions.
 */
TestSuite(tc_tvwi_test08tvw,
        .init = setup,
        .fini = teardown);

/**
 * Check disk IO related functionality.
 */
Test(tc_tvwi_test08tvw, fileops) {
    cr_expect(eq(str, test08.file->filename, "test/resources/test08.tvw"),
            "wrapper file has wrong filename!");
    cr_expect(eq(ptr, test08.file->fp, NULL),
            "wrapper file pointer is still open!");
}

/**
 * Check the header data was read correctly, against known-good values.
 */
Test(tc_tvwi_test08tvw, header) {
    cr_expect(eq(u32, test08.file->header.format_version, 0),
            "wrong format version!");
    cr_expect(eq(u64, test08.file->header.start_of_contents_gs, 111),
            "wrong SOC GS offset!");
    cr_expect(eq(u64, test08.file->header.len_of_contents, 213),
            "wrong contents length!");
    cr_expect(eq(u16, test08.file->header.comp_algo_meta, 1),
            "wrong metadata compression algo!");
    cr_expect(eq(u16, test08.file->header.comp_algo_file, 1),
        "wrong contents compression algo!");
    uint8_t good[] = {
        0x0b, 0x80, 0x81, 0xb1, 0xe7, 0xe4, 0x01, 0x4a,
        0x4f, 0x8d, 0xe9, 0x2f, 0x18, 0x60, 0x59, 0x53,
        0xdc, 0x27, 0x04, 0xac, 0x1d, 0x31, 0xb7, 0x3f,
        0xcb, 0x01, 0x6f, 0x86, 0xb6, 0x75, 0x16, 0xdd,
        0x88, 0x41, 0xfb, 0x7f, 0xf8, 0x9b, 0xd8, 0xe9,
        0xbd, 0x93, 0x5c, 0x80, 0xfb, 0x05, 0x78, 0xbf,
        0x42, 0x79, 0x7b, 0xe4, 0x02, 0x68, 0x6e, 0x4d,
        0x77, 0x87, 0x86, 0xfc, 0x77, 0x18, 0x00, 0x53
    };
    cr_expect(eq(u8[64], test08.file->header.sha512, good),
            "wrong file contents hash!");
}

/**
 * Check the test08.tvw metadata was read correctly, against known-good values.
 */
Test(tc_tvwi_test08tvw, metadata) {
    cr_expect(eq(ptr, test08.file->xmlerr, NULL),
            "xmlerr is not NULL, parsing failed!");
    cr_assert(ne(ptr, test08.file->xmlroot, NULL),
            "xmlroot is NULL, parsing *really* failed!");
    cr_expect(eq(sz, test08.file->sizeof_meta, 23),
            "did not read correct metadata size!");
    cr_assert(ne(ptr, test08.file->metadata, NULL),
            "metadata is NULL; something went wrong!");

    char good[] = \
"<tvm>\n"\
"Metadata!\n"\
"</tvm>\n";

    cr_assert(eq(sz, strlen(good), 23),
            "wrong GOOD size: %ld!", strlen(good));
    cr_assert(eq(str, test08.file->metadata, good),
            "(raw) metadata is incorrect!");
}

/**
 * Check the test08.tvw file contents were read correctly, against known-good
 * values.
 */
Test(tc_tvwi_test08tvw, contents) {
    cr_expect(ne(ptr, test08.file->contents, NULL),
            "file contents is NULL, something failed!");
    cr_expect(eq(sz, test08.file->sizeof_cont, 213),
            "contents size is incorrect!");

    char good[] = "# TagVFS\n\nI need to put all my photos somewhere.\n" \
                   "\n* [Documentation][docs]\n\n[docs]: doc/\n\n" \
                   "# Contributions\n\nGit Flow model for core contributors;" \
                   " GitHub fork-and-pull model for non-core\n" \
                   "contributors.  All welcome!\n\n";
    cr_expect(eq(str, test08.file->contents, good),
            "contents are incorrect!");
}

