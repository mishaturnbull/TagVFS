/* TagVFS compression table headers.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * This file provides function & type declarations for compression tables.
 */

#pragma once

#include <stdio.h>

#include "comp_passthru.h"

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
extern struct COMPRESSION_ALGO COMP_TABLE[];

extern size_t SIZEOF_COMP_TABLE;

/**
 * Return a human-readable table of compression algorithms.
 */
int tvcomp_make_table(size_t *outsize, char **outbuf);

