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

    algo = lookup_algo_by_id(none);

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

Test(tvc_funcs, compress) {
#ifdef CONFIG_TVCALG_TABLETEST
    size_t insize = 5;
    char *input = "Hello";
    size_t outsize;
    char *output;
    size_t expoutsize = 5;
    char *expout = "Ifmmp";

    int err = compress(1, &insize, &input, &outsize, &output);

    cr_expect(err == 0,
            "generic compress() returned err %d", err);
    cr_expect(outsize == expoutsize,
            "generic compress() output wrong size (%zu != %zu)",
            outsize, expoutsize);
    cr_expect_str_eq(output, expout,
            "generic compress() output wrong (%s VS %s)", input, output);
#else
    cr_fail("tabletest algo disabled -- cannot perform compress() test!");
#endif
}

Test(tvc_funcs, decompress) {
#ifdef CONFIG_TVCALG_TABLETEST
    size_t insize = 5;
    char *input = "Ifmmp";
    size_t outsize;
    char *output;
    size_t expoutsize = 5;
    char *expout = "Hello";

    int err = decompress(1, &insize, &input, &outsize, &output);

    cr_expect(err == 0,
            "generic compress() returned err %d", err);
    cr_expect(outsize == expoutsize,
            "generic compress() output wrong size (%zu != %zu)",
            outsize, expoutsize);
    cr_expect_str_eq(output, expout,
            "generic compress() output wrong (%s VS %s)", input, output);
#else
    cr_fail("tabletest algo disabled -- cannot perform decompress() test!");
#endif
}


