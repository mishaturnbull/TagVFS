/* TagVFS compression algorithm tables.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * This file provides the compression and decompression algorithm tables.
 */

#include "tables.h"

struct COMPRESSION_ALGO comp_table[] = {
    { &passthru_func,
      &passthru_func,
      "No compression" },

    { &effectivity_test_comp,
      &effectivity_test_decomp,
      "Compression table test" }
};

int compress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf) {
    struct COMPRESSION_ALGO algoent = comp_table[algo];
    return (*(algoent.compress))(insize, inbuf, outsize, outbuf);
}

int decompress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf) {
    struct COMPRESSION_ALGO algoent = comp_table[algo];
    return (*(algoent.decompress))(insize, inbuf, outsize, outbuf);
}

