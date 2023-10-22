/* TagVFS compression algorithm tables.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * This file provides the compression and decompression algorithm tables.
 */

#include "compression_table.h"

struct COMPRESSION_ALGO COMP_TABLE[] = {
    { &passthru_func,
      &passthru_func,
      "No compression" },

    { &effectivity_test_comp,
      &effectivity_test_decomp,
      "Compression table test" }
};

size_t SIZEOF_COMP_TABLE = (size_t)(sizeof(COMP_TABLE) / sizeof(COMP_TABLE[0]));

//                         0         1         2         3         4         5
//                         012345678901234567890123456789012345678901234567890
#define TVCOMP_TBL_ROWSEP "+------+------------------------------------------+\n"
#define TVCOMP_TBL_HDRROW "| ID   | Algorithm                                |\n"
#define TVCOMP_TBL_HDRSEP "+======+==========================================+\n"
#define TVCOMP_TBL_ROW    "| % 4ld | %-40.40s |\n"
#define TVCOMP_TBL_ROWLEN strlen(TVCOMP_TBL_ROWSEP)+1

int tvcomp_make_table(size_t *outsize, char **outbuf) {

    size_t nlines = 3 + SIZEOF_COMP_TABLE * 2;
    *outsize = (strlen(TVCOMP_TBL_ROWSEP) + 1) * nlines;
    *outbuf = (char*)calloc(*outsize, sizeof(char));

    strncat(*outbuf, TVCOMP_TBL_ROWSEP, TVCOMP_TBL_ROWLEN);
    strncat(*outbuf, TVCOMP_TBL_HDRROW, TVCOMP_TBL_ROWLEN);
    strncat(*outbuf, TVCOMP_TBL_HDRSEP, TVCOMP_TBL_ROWLEN);

    char *tmpline = (char*)calloc(TVCOMP_TBL_ROWLEN, sizeof(char));
    for (size_t i = 0; i < SIZEOF_COMP_TABLE; i++) {
        snprintf(tmpline, TVCOMP_TBL_ROWLEN, TVCOMP_TBL_ROW, i,
                COMP_TABLE[i].human_name);
        strncat(*outbuf, tmpline, TVCOMP_TBL_ROWLEN);
        strncat(*outbuf, TVCOMP_TBL_ROWSEP, TVCOMP_TBL_ROWLEN);
    }
    free(tmpline);

    return 0;
}

