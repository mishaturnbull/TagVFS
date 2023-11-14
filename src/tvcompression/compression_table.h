/* TagVFS compression table headers.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * This file provides function & type declarations for compression tables.
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debugs.h"

#if defined(CONFIG_TVCALG_NOCOMPRESSION) || defined(CONFIG_TVCALG_TABLETEST)
#include "comp_passthru.h"
#endif

/**
 * Compression algorithm descriptor table entry
 *
 * This struct comprises each entry in the compression algorithm table.  It
 * includes pointers to functions for encryption and decryption, as well as a
 * human-readable variant of the name of the compression algorithm.
 */
struct COMPRESSION_ALGO {
    /** Algorithm identifier. */
    uint16_t id;

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
 * transparent (de)compression ability.  It has a length of
 * :c:var:`SIZEOF_COMP_TABLE`.
 *
 * .. warning::
 *
 *    The index of an algorithm in this table is **NOT** necessarily equivalent
 *    to its ID!  See the :c:func:`lookup_algo_by_id` func for getting the
 *    right structure.
 */
extern struct COMPRESSION_ALGO COMP_TABLE[];

/**
 * The length of :c:var:`COMP_TABLE`.
 *
 * Useful if you want to iterate through it for some reason (though, if you
 * find yourself in that situation, think very carefully about whether there's
 * a better way to solve your problem).
 */
extern size_t SIZEOF_COMP_TABLE;

// turns out it's relatively easy to produce a valid ReStructuredText format
// table!  just need to arrange for a separator line after every entry, and
// space things out just so.
//                                  1         2         3         4         5
//                         1234567890123456789012345678901234567890123456789012
#define TVCOMP_TBL_ROWSEP "+------+------------------------------------------+\n"
#define TVCOMP_TBL_HDRROW "| ID   | Algorithm                                |\n"
#define TVCOMP_TBL_HDRSEP "+======+==========================================+\n"
#define TVCOMP_TBL_ROW    "| % 4d | %-40.40s |\n"
#define TVCOMP_TBL_ROWLEN strlen(TVCOMP_TBL_ROWSEP)

/**
 * Return a human-readable table of compression algorithms.
 *
 * It also happens that this function's output is valid ReStructuredText,
 * should you ever want to do something with that...
 *
 * :param outsize: Pointer to a ``size_t`` that will be set to the size of the
 *    table character buffer.  This will be SET by the function (output
 *    parameter).
 * :param outbuf: Pointer to a ``char*`` that will be set to the string
 *    containing the algorithm table.  The string will most likely contain
 *    newlines.  This will be ALLOCATED AND WRITTEN by the function (output
 *    parameter).  Its length is returned via the ``outsize`` parameter.
 * :return: Error code, or 0 on success.
 */
int tvcomp_make_table(size_t *outsize, char **outbuf);

