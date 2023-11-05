/* TagVFS compression table unit tests.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-30
 *
 * This file contains unit tests for items declared in compression_table.c.
 */

#include "../unittest.h"
#include "tvcompression/compression_table.h"

#ifdef CONFIG_TVCALG_TABLETEST
#include "tvcompression/tvcompression.h"
#include "tvcompression/comp_passthru.h"
#endif

Test(tvc_algos, nocompression) {
#ifdef CONFIG_TVCALG_NOCOMPRESSION
    struct COMPRESSION_ALGO *plain = lookup_algo_by_id(0);

    cr_expect(eq(u16, plain->id, 0),
            "expected plain ID 0, did not find it!");
    cr_expect(plain->compress == &(passthru_func),
            "nocomp has wrong compression function!");
    cr_expect(plain->decompress == &(passthru_func),
            "nocomp has wrong decompression function!");
    cr_expect(eq(str, plain->human_name, "No compression"),
            "wrong human-readable name for nocompression algo");
#else
    // fail builds that don't allow uncompressed data
    cr_fail("no-compression passthru has been disabled!!");
#endif
}

/**
 * Test the compression table exists and the lookup function can look up
 * algorithms.
 *
 * Here, we test that the algorithm returned by the lookup algo is the right
 * algorithm, focusing on confirming the functions.  For this test, we don't
 * care that the functions within the table do the right thing -- that's tested
 * elsewhere.
 *
 * necessary header file is tvcompression/comp_passthru.c -- already included
 * unconditionally at the top (because it also contains the nocompression
 * functions)
 */
Test(tvc_algos, tabletest) {
#ifdef CONFIG_TVCALG_TABLETEST
    struct COMPRESSION_ALGO *algo = lookup_algo_by_id(1);

    cr_expect(eq(u16, algo->id, 1),
            "table test algorithm has wrong ID %d", algo->id);
    cr_expect(algo->compress == &(effectivity_test_comp),
            "tabletest has wrong compression function!");
    cr_expect(algo->decompress == &(effectivity_test_decomp),
            "tabletest has wrong decompression function!");
    cr_expect(eq(str, algo->human_name, "Compression table test"),
            "tabletest has wrong human name!");
#else
    cr_skip("tabletest algo is disabled -- cannot perform table test!");
#endif
}

/**
 * Test the generation of the ASCII art table produced by the tvctool exe.
 *
 * This doesn't so much check that the contents are correct, because ... that's
 * not really doable without just re-implementing the table maker here.  But,
 * we can check buffer sizes and whatnot.
 */
Test(tvc_funcs, maketable) {
    size_t outsize;
    char *outbuf;
    int err = tvcomp_make_table(&outsize, &outbuf);

    //               (  length of one line   ) * (      number of lines      )
    size_t expsize = strlen(TVCOMP_TBL_ROWSEP) * (3 + (2 * SIZEOF_COMP_TABLE));

    cr_expect(eq(int, err, 0),
            "maketable() returned nonzero: %d", err);
    cr_expect(eq(sz, strlen(outbuf), outsize),
            "algo table strlen not equal to reported size");
    cr_expect(eq(sz, outsize, expsize),
            "algo table reported size != expected");
}


