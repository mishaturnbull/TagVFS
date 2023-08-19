/* Types and constants for TVW files (wrappers).
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-19
 *
 * This file defines structs for handling TagVFS wrapper files.
 */

#pragma once

#include <stdint.h>
#include <stdio.h>

#include <libxml/xmlmemory.h>

/**
 * Wrapper file header structure.
 */
typedef struct WRAPPER_FILE_HEADER {
    /** Wrapper format version number. */
    uint32_t format_version;

    /** Start-of-contents GS character (``0x1d``) offset in file. */
    uint64_t start_of_contents_gs;

    /** Length of file contents (bytes). */
    uint64_t len_of_contents;

    /** Compression algorithm in use for metadata. */
    uint16_t comp_algo_meta;

    /** Compression algorithm in use for file contents. */
    uint16_t comp_algo_file;

    /** SHA-512 digest (512 *bit*, not *byte*). */
    uint8_t sha512 [512 / 8];
} WRAPPER_FILE_HEADER;

/**
 * Wrapper file structure.
 */
typedef struct WRAPPER_FILE {
    /**
     * File pointer for on-disk IO.
     * **For internal use only**
     */
    FILE *fp;

    /** Filename of on-disk IO. */
    char *filename;

    /** Header structure information. */
    WRAPPER_FILE_HEADER header;

    /** Pointer to metadata character array. */
    char *metadata;

    /** Length of the metadata character array. */
    size_t sizeof_meta;

    /** pointer to libxml2 structure root */
    xmlDocPtr xmlroot;

    /** Pointer to the file contents character array. */
    char *contents;

    /** Length of the file contents character array. */
    size_t sizeof_cont;
} WRAPPER_FILE;

