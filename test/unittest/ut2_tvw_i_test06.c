/* tvw_i unit tests
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-26
 *
 * Unit tests for tvw_i library.
 */

#include "unittest.h"

#include "tvwio/tvw_i.h"

struct test06 {
    struct WRAPPER_FILE *file;
    int err;
} test06;

void setup_test06tvw(void) {
    test06.file = (struct WRAPPER_FILE*)calloc(1,
            sizeof(struct WRAPPER_FILE));
    test06.err = read_wrapper("test/resources/test06.tvw",
            test06.file);
    cr_assert(test06.err == 0,
            "got nonzero return code from read_wrapper!");
}
void teardown_test06tvw(void) {
    tvwfree(test06.file);
}

TestSuite(tvwi_test06, .init = setup_test06tvw, .fini = teardown_test06tvw);

Test(tvwi_test06, fileops) {
    cr_expect(test06.file->fp == NULL,
            "wrapper file pointer is still open!");
}

Test(tvwi_test06, header) {
    cr_expect(test06.file->header.format_version == 0,
            "wrong format version!");
    cr_expect(test06.file->header.start_of_contents_gs == 131,
            "wrong SOC GS offset!");
    cr_expect(test06.file->header.len_of_contents == 213,
            "wrong contents length!");
    cr_expect(test06.file->header.comp_algo_meta == 0,
            "wrong metadata compression algo!");
    cr_expect(test06.file->header.comp_algo_file == 0,
        "wrong contents compression algo!");
 uint8_t good[] = {
        0x0b, 0x80, 0x81, 0xb1, 0xe7, 0xe4, 0x01, 0x4a, 0x4f, 0x8d, 0xe9, 0x2f,
        0x18, 0x60, 0x59, 0x53, 0xdc, 0x27, 0x04, 0xac, 0x1d, 0x31, 0xb7, 0x3f,
        0xcb, 0x01, 0x6f, 0x86, 0xb6, 0x75, 0x16, 0xdd, 0x88, 0x41, 0xfb, 0x7f,
        0xf8, 0x9b, 0xd8, 0xe9, 0xbd, 0x93, 0x5c, 0x80, 0xfb, 0x05, 0x78, 0xbf,
        0x42, 0x79, 0x7b, 0xe4, 0x02, 0x68, 0x6e, 0x4d, 0x77, 0x87, 0x86, 0xfc,
        0x77, 0x18, 0x00, 0x53 
    };
    cr_expect_arr_eq(test06.file->header.sha512, good, 64,
            "wrong file contents hash!");
}

Test(tvwi_test06, metadata) {
    cr_expect(test06.file->xmlerr == NULL,
            "xmlerr is not NULL, parsing failed!");
    cr_assert(test06.file->xmlroot != NULL,
            "xmlroot is NULL, parsing *really* failed!");
    cr_expect(test06.file->sizeof_meta == 732,
            "did not read correct metadata size!");
    cr_assert(test06.file->metadata != NULL,
            "metadata is NULL; something went wrong!");

    char good[] = \
"   <tvm>\n"\
"       <tags>\n"\
"           <tag>Christmas</tag>\n"\
"           <tag>Animals<tag>Dog</tag></tag>\n"\
"       </tags>\n"\
"       <stat>\n"\
"           <name>Untitled_final (1) USE THIS ONE (1) (18).docx.jpg</name>\n"\
"           <permission>0644</permission>\n"\
"           <uid>1000</uid>\n"\
"           <gid>1000</gid>\n"\
"           <access>1234567890</access>\n"\
"           <modify>1234567890</modify>\n"\
"           <change>1234567890</change>\n"\
"           <birth>1234567890</birth>\n"\
"       </stat>\n"\
"       <fromfile format=\"application/xml\" from=\"exif\">\n"\
"           <ExposureProgram>28/5</ExposureProgram>\n"\
"           <CameraModel>Banana</CameraModel>\n"\
"       </fromfile>\n"\
"       <user>\n"\
"           Hello, world!\n"\
"           <b>Bold, maybe?</b>\n"\
"       </user>\n"\
"   </tvm>\n\n";

    cr_assert(strlen(good) == 732,
            "wrong GOOD size: %ld!", strlen(good));
    cr_expect_str_eq(test06.file->metadata, good,
            "(raw) metadata is incorrect!");

}

Test(tvwi_test06, contents) {
    cr_expect(test06.file->contents != NULL,
            "file contents is NULL, something failed!");
    cr_expect(test06.file->sizeof_cont == 213,
            "contents size is incorrect!");

    char good[] = "# TagVFS\n\nI need to put all my photos somewhere.\n" \
                   "\n* [Documentation][docs]\n\n[docs]: doc/\n\n" \
                   "# Contributions\n\nGit Flow model for core contributors;" \
                   " GitHub fork-and-pull model for non-core\n" \
                   "contributors.  All welcome!\n\n";
    cr_expect_str_eq(test06.file->contents, good,
            "contents are incorrect!");
}

