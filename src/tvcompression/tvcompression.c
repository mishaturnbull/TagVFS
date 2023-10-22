/* tvcompression main entry functions.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-19
 *
 * Main entry function for TagVFS compression calls.
 */

#include "tvcompression.h"

/*
 * All that really needs to happen in these two functions is selection of the
 * right algorithm, then pass out the call to that algo's .compress or
 * .decompress function as appropriate.
 *
 * However, I thought it best to implement generic (non-algo-specific) dispatch
 * functions in case in the future the storage of algorithms is changed.  Then,
 * only the method with which these two functions look up the algorithm must be
 * updated -- not everywhere else that uses the compression functionality.
 */

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

