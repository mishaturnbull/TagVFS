/* tvcompression main entry functions.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-19
 *
 * Main entry function for TagVFS compression calls.
 */

#include "tvcompression.h"

int compress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf) {
    struct COMPRESSION_ALGO algoent = COMP_TABLE[algo];
    return (*(algoent.compress))(insize, inbuf, outsize, outbuf);
}

int decompress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf) {
    struct COMPRESSION_ALGO algoent = COMP_TABLE[algo];
    return (*(algoent.decompress))(insize, inbuf, outsize, outbuf);
}

