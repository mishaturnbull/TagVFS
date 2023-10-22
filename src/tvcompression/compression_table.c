/* TagVFS compression algorithm tables.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * This file provides the compression and decompression algorithm tables.
 */

#include "compression_table.h"

/*
 * Do NOT add any index counts to this sourcecode!
 *
 * Manually adjusting them every time an algorithm is inserted / removed would
 * be really, really annoying.  Instead, use the `tvctool` binary to query
 * indexes.  That's literally what I originally made it for -- to print out the
 * indexed table for you, so you don't have to count by hand.
 */
struct COMPRESSION_ALGO COMP_TABLE[] = {
    { &passthru_func,
      &passthru_func,
      "No compression" },

    { &effectivity_test_comp,
      &effectivity_test_decomp,
      "Compression table test" }
};

// yes, you can do this sizeof a pointer -- but only when the pointer is
// allocated statically at compile-time.
size_t SIZEOF_COMP_TABLE = (size_t)(sizeof(COMP_TABLE) / sizeof(COMP_TABLE[0]));

// turns out it's relatively easy to produce a valid ReStructuredText format
// table!  just need to arrange for a separator line after every entry, and
// space things out just so.
//                         0         1         2         3         4         5
//                         012345678901234567890123456789012345678901234567890
#define TVCOMP_TBL_ROWSEP "+------+------------------------------------------+\n"
#define TVCOMP_TBL_HDRROW "| ID   | Algorithm                                |\n"
#define TVCOMP_TBL_HDRSEP "+======+==========================================+\n"
#define TVCOMP_TBL_ROW    "| % 4ld | %-40.40s |\n"
#define TVCOMP_TBL_ROWLEN strlen(TVCOMP_TBL_ROWSEP)+1

int tvcomp_make_table(size_t *outsize, char **outbuf) {

    // allocate output buffer.  we always have the first the lines -- a rowsep,
    // hdrrow, and hdrsep
    size_t nlines = 3 + SIZEOF_COMP_TABLE * 2;
    *outsize = (TVCOMP_TBL_ROWLEN + 1)* nlines;
    *outbuf = (char*)calloc(*outsize, sizeof(char));

    // write the first 3 lines
    strncat(*outbuf, TVCOMP_TBL_ROWSEP, TVCOMP_TBL_ROWLEN);
    strncat(*outbuf, TVCOMP_TBL_HDRROW, TVCOMP_TBL_ROWLEN);
    strncat(*outbuf, TVCOMP_TBL_HDRSEP, TVCOMP_TBL_ROWLEN);

    // allocate space for a temp line to snprintf into
    char *tmpline = (char*)calloc(TVCOMP_TBL_ROWLEN, sizeof(char));

    // for every algorithm...
    for (size_t i = 0; i < SIZEOF_COMP_TABLE; i++) {

        // format the contents row (the interesting one)
        snprintf(tmpline, TVCOMP_TBL_ROWLEN, TVCOMP_TBL_ROW, i,
                COMP_TABLE[i].human_name);
        strncat(*outbuf, tmpline, TVCOMP_TBL_ROWLEN);
        
        // and then add a row separator
        strncat(*outbuf, TVCOMP_TBL_ROWSEP, TVCOMP_TBL_ROWLEN);
    }

    free(tmpline);
    return 0;
}

