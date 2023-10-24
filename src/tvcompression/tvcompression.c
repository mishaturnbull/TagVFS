/* tvcompression main entry functions.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-19
 *
 * Main entry function for TagVFS compression calls.
 */

#include "tvcompression.h"

// provide an interface to look up an algorithm by ID
// TODO: make this faster.  i feel like there should be a way
//
// but also, don't break the ability to kconfig-out algorithms from the table
struct COMPRESSION_ALGO * lookup_algo_by_id(uint16_t algo) {
    size_t entry;

    char found = 0;
    for (size_t i = 0; i < SIZEOF_COMP_TABLE; i++) {
        if (COMP_TABLE[i].id == algo) {
            entry = (size_t)i;
            found = 1;
            break;
        }
    }
    if (found == 0) {
        TV_LOGW("WARNING: algo %d unknown!  using first entry!\n", algo);
        entry = 0;
    }

    return &(COMP_TABLE[entry]);
}


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
    struct COMPRESSION_ALGO *algoent = lookup_algo_by_id(algo);
    return (*(algoent->compress))(insize, inbuf, outsize, outbuf);
}

int decompress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf) {
    struct COMPRESSION_ALGO *algoent = lookup_algo_by_id(algo);
    return (*(algoent->decompress))(insize, inbuf, outsize, outbuf);
}

