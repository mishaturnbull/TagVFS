/* TagVFS compression algorithm: passthrough (non-compressed)
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * This file contains sourcecode for the most basic compression algorithm (none)
 */

#include "comp_passthru.h"

/*
 * This function needs to just create a new buffer for the output, copy the
 * contents unaltered into it, and return as fast as possible.
 *
 * Yes... we do still have to make a new buffer here.  It's common practice for
 * the caller of compression/decompression functions to free the inbuf, and if
 * we were to just set outbuf=inbuf here, freeing the input buffer would then
 * free the output buffer -- which the callers don't expect, and could have
 * disasterous effects.
 *
 * (Since there's no difference between compression and decompression in this
 * mode, no need to define another function for decomp.  We'll just use this
 * func for both.)
 *
 * Note to future self: if you can come up with a faster way of doing this, do
 * it!
 */
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

/*
 * Per the declared spec, this function is a shift-1 Caesar cipher.
 *
 * We'll just copy the data over one byte at a time, adding one as we go.
 */
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

/*
 * This function is a perfect reverse of the previous.
 */
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


