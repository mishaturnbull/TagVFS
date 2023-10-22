/* TagVFS  compression algo header: passthru (no compression)
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * Header for the passthru (no) compression algo.
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * Pass data through unaltered.  No compression applied.
 *
 * This function provides a no-compression option as the default "compression"
 * algorithm (quotes, because it doesn't compress anything).  This exists
 * because having a do-nothing option at index 0 of the compression algo table
 * is a much cleaner solution than special-casing ``comp_algo_x == 0``
 * throughout the entire program.
 *
 * Generally, this function should not be called directly.  See
 * :c:func:`compress` and :c:func:`decompress`.
 */
int passthru_func(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

/**
 * Ensure that data is flowing through the compression table functions.
 *
 * This function mangles data supplied by adding ``1`` to the ordinal value of
 * every character in the input buffer as it copies it to the output buffer.
 * In effect, a shift-1 Ceasar cipher.
 *
 * The effect of this function is precisely undone by
 * :c:func:`effectivity_test_decomp`.
 *
 * Generally, this function should not be called directly.  See
 * :c:func:`compress`.
 */
int effectivity_test_comp(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

/**
 * Ensure that data is flowing through the decompression table functions.
 *
 * This function mangles data supplied by adding ``1`` to the ordinal value of
 * every character in the input buffer as it copies it to the output buffer.
 * In effect, a shift-negative1 Ceasar cipher.
 *
 * The effect of this function is precisely undone by
 * :c:func:`effectivity_test_comp`.
 *
 * Generally, this function should not be called directly.  See
 * :c:func:`decompress`.
 */
int effectivity_test_decomp(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

