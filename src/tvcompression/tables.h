/* TagVFS compression table headers.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * This file provides function & type declarations for compression tables.
 */

#pragma once

#include <stdint.h>
#include <stdint.h>
#include "passthru.h"

/**
 * Compression algorithm descriptor table entry
 *
 * This struct comprises each entry in the compression algorithm table.  It
 * includes pointers to functions for encryption and decryption, as well as a
 * human-readable variant of the name of the compression algorithm.
 */
struct COMPRESSION_ALGO {
    /** Pointer to compression function. */
    int (*compress)(size_t*, char**, size_t*, char**);

    /** Pointer to decompression function. */
    int (*decompress)(size_t*, char**, size_t*, char**);

    /** Human-sensible name of this algorithm. */
    char *human_name;
};

/**
 * Master table of compression algorithms.
 *
 * This array defines all compression algorithms supported by TagVFS for its
 * transparent (de)compression ability.  The ID of any given algorithm is its
 * index in this array.
 */
extern struct COMPRESSION_ALGO comp_table[];

/**
 * Generic transparent compression function.
 *
 * Given an algorithm ID and necessary arguments, invokes the specified
 * algorithm's compression function.  This should be the main (and only!) call
 * to compress data for TagVFS wrapper files.
 *
 * :param algo: ID of the compression algorithm to utilize for this operation.
 * :param insize: Pointer to size of the input data (uncompressed).  This will
 *    not be altered by the function (input parameter).
 * :param inbuf: Pointer to the character array of the input data
 *    (uncompressed).  This will not be altered by the function (input 
 *    parameter).
 * :param outsize: Pointer to the size of the output data (compressed).  This
 *    will be SET by the function (output parameter).
 * :param outbuf: Pointer to the character array of the output data buffer
 *    (compressed).  This will be INITIALIZED AND WRITTEN by the function (output
 *    parameter).  Its length is returned via the ``outsize`` parameter.  The
 *    caller is responsible for freeing this buffer when no longer needed.
 * :return: Error code, or 0 on success.
 */
int compress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

/**
 * Generic transparent decompression function.
 *
 * Given an algorithm ID and necessary arguments, invokes the specified
 * algorithm's decompression function.  This should be the main (and only!)
 * call to decompress data for TagVFS wrapper files.
 *
 * :param algo: ID of the decompression algorithm to utilize for this operation.
 * :param insize: Pointer to size of the input data (compressed).  This will
 *    not be altered by the function (input parameter).
 * :param inbuf: Pointer to the character array of the input data
 *    (compressed).  This will not be altered by the function (input 
 *    parameter).
 * :param outsize: Pointer to the size of the output data (decompressed).  This
 *    will be SET by the function (output parameter).
 * :param outbuf: Pointer to the character array of the output data buffer
 *    (decompressed).  This will be INITIALIZED AND WRITTEN by the function
 *    (output parameter).  Its length is returned via the ``outsize`` parameter.
 *    The caller is responsible for freeing this buffer when no longer needed.
 * :return: Error code, or 0 on success.
 */
int decompress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

