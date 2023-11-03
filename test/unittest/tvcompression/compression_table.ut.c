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

// necessary header file is tvcompression/comp_passthru.c -- already included
// unconditionally at the top (because it also contains the nocompression
// functions)
Test(tvc_algos, tabletest) {
#ifdef CONFIG_TVCALG_TABLETEST
    struct COMPRESSION_ALGO *algo = lookup_algo_by_id(1);

    cr_expect(algo->id == 1,
            "table test algorithm has wrong ID %d", algo->id);
    cr_expect(algo->compress == &(effectivity_test_comp),
            "tabletest has wrong compression function!");
    cr_expect(algo->decompress == &(effectivity_test_decomp),
            "tabletest has wrong decompression function!");
    cr_expect_str_eq(algo->human_name, "Compression table test",
            "tabletest has wrong human name!");
#else
    cr_fail("tabletest algo is disabled -- cannot perform table test!");
#endif
}

Test(tvc_funcs, maketable) {
    size_t outsize;
    char *outbuf;
    int err = tvcomp_make_table(&outsize, &outbuf);

    //               (  length of one line   ) * (      number of lines      )
    size_t expsize = strlen(TVCOMP_TBL_ROWSEP) * (3 + (2 * SIZEOF_COMP_TABLE));

    cr_expect(err == 0,
            "maketable() returned nonzero: %d", err);
    cr_expect(strlen(outbuf) == outsize,
            "algo table strlen not equal to reported size (%zu != %zu)",
            strlen(outbuf), outsize);
    cr_expect(outsize == expsize,
            "algo table reported size != expected (%zu != %zu)",
            outsize, expsize);
}


