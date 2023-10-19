/* TagVFS compression algorithm: passthrough (non-compressed)
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * This file contains sourcecode for the most basic compression algorithm (none)
 */

#include "passthru.h"

int passthru_func(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf) {

    *outbuf = (char*)calloc(*insize, sizeof(char));
    *outsize = *insize;
    memcpy(*outbuf, *inbuf, *insize);

    return 0;
}

int effectivity_test_comp(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf) {

    *outbuf = (char*)calloc(*insize, sizeof(char));
    *outsize = *insize;

    for (size_t i = 0; i < *outsize; i++ ) {
        (*outbuf)[i] = (*inbuf)[i] + 1;
    }

    return 0;
}

int effectivity_test_decomp(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf) {

    *outbuf = (char*)calloc(*insize, sizeof(char));
    *outsize = *insize;

    for (size_t i = 0; i < *outsize; i++ ) {
        (*outbuf)[i] = (*inbuf)[i] - 1;
    }

    return 0;
}


