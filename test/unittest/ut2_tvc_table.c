/* tvc table unit tests
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-06-27
 *
 * Unit tests for tvcompression library table.
 */

#include "unittest.h"

#include "tvcompression/tvcompression.h"
#include "config.h"

#include "tvcompression/comp_passthru.h"

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
    
    cr_expect(algo->id == first->id,
            "act-when-noalg did not return first algo ID!  (%d != %d)",
            algo->id, first->id);
    // check the *actual address* of the human_name string.  this will also
    // tell us whether we got truly the right element back
    cr_expect(&(algo->human_name) == &(first->human_name),
            "act-when-noalg did not return first enabled algorithm: %s VS %s",
            algo->human_name, first->human_name);
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

