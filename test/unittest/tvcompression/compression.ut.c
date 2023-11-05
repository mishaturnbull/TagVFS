/* TagVFS compression unit tests.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-11-03
 *
 * This file contains unit tests for items declared in tvcompression.c.
 */

#include "../unittest.h"
#include "tvcompression/tvcompression.h"

#ifdef CONFIG_TVCALG_TABLETEST
#include "tvcompression/comp_passthru.h"
#endif

/**
 * Test the case where the desired algorithm ID is not found in the compression
 * algorithm table.
 *
 * First, we need to *identify* such an algorithm ID -- iterate through the
 * table to find an ID that's not there.  Then, ask the lookup function for
 * that (nonexistent) algorithm, and confirm it gives us the first one in the
 * table.
 */
Test(tvc_funcs, algonotfound) {
	uint16_t none = 0;
    char exists = 1;
    size_t i;
    struct COMPRESSION_ALGO *algo;
    while (exists) {
        exists = 0;
        for (i = 0; i < SIZEOF_COMP_TABLE; i++) {
            algo = &(COMP_TABLE[i]);
            if (algo->id == none) {
                none += 1;
                exists = 1;
            }
        }
    }

	cr_redirect_stderr();

    algo = lookup_algo_by_id(none);

    cr_assert_stderr_eq_str("WARNING: algo 2 unknown!  using first entry!\n",
            "did not emit correct warning message!");

    struct COMPRESSION_ALGO *first = &(COMP_TABLE[0]);

    cr_expect(eq(u16, algo->id, first->id),
            "act-when-noalg did not return first algo ID!  (%d != %d)",
            algo->id, first->id);
    // check the *actual address* of the human_name string.  this will also
    // tell us whether we got truly the right element back
    cr_expect(eq(ptr, &(algo->human_name), &(first->human_name)),
            "act-when-noalg did not return first enabled algorithm: %s VS %s",
            algo->human_name, first->human_name);
}

/**
 * Test the generic compression function.
 *
 * We're less concerned here with the compression algorithm itself, and more so
 * that the *right* compression algorithm is identified and used.  The actual
 * algorithms are tested in-depth in their own unit test suites.
 */
Test(tvc_funcs, compress) {
#ifdef CONFIG_TVCALG_TABLETEST
    size_t insize = 5;
    char *input = "Hello";
    size_t outsize;
    char *output;
    size_t expoutsize = 5;
    char *expout = "Ifmmp";

    int err = compress(1, &insize, &input, &outsize, &output);

    cr_expect(eq(int, err, 0),
            "generic compress() returned non-zero err");
    cr_expect(eq(sz, outsize, expoutsize),
            "generic compress() output wrong size");
    cr_expect(eq(str, output, expout),
            "generic compress() output is wrong");
#else
    cr_fail("tabletest algo disabled -- cannot perform compress() test!");
#endif
}

/**
 * Test the generic decompression function.
 *
 * As with the generic compression unit test, we're less concerned here with
 * the decompression *algorithm*, and more so that the *right* decompression
 * algo is selected and used.
 */
Test(tvc_funcs, decompress) {
#ifdef CONFIG_TVCALG_TABLETEST
    size_t insize = 5;
    char *input = "Ifmmp";
    size_t outsize;
    char *output;
    size_t expoutsize = 5;
    char *expout = "Hello";

    int err = decompress(1, &insize, &input, &outsize, &output);

    cr_expect(eq(int, err, 0),
            "generic compress() returned non-zero err");
    cr_expect(eq(sz, outsize, expoutsize),
            "generic compress() output wrong size");
    cr_expect(eq(str, output, expout),
            "generic compress() output wrong");
#else
    cr_fail("tabletest algo disabled -- cannot perform decompress() test!");
#endif
}


