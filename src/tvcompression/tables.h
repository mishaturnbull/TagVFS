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
 */
struct COMPRESSION_ALGO {
    int (*compress)(size_t*, char**, size_t*, char**);
    int (*decompress)(size_t*, char**, size_t*, char**);
    char *human_name;
};

extern struct COMPRESSION_ALGO comp_table[];

int compress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);
int decompress(uint16_t algo,
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

