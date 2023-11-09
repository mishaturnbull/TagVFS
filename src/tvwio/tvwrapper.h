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
struct WRAPPER_FILE_HEADER {
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
};

/**
 * Wrapper file structure.
 */
struct WRAPPER_FILE {
    /**
     * File pointer for on-disk IO.
     * **For internal use only**
     */
    FILE *fp;

    /** Filename of on-disk IO. */
    char *filename;

    /** Header structure information. */
    struct WRAPPER_FILE_HEADER header;

    /** Pointer to metadata character array. */
    char *metadata;

    /** Length of the metadata character array. */
    size_t sizeof_meta;

    /** pointer to libxml2 structure root */
    xmlDocPtr xmlroot;

    /** pointer to libxml2 parser error, if any */
    xmlErrorPtr xmlerr;

    /** Pointer to the file contents character array. */
    char *contents;

    /** Length of the file contents character array. */
    size_t sizeof_cont;
};

/**
 * Free all the members of a wrapper file structure.
 *
 * .. warning::
 *
 *    This function does *NOT* call the final :c:`free(file)`!  
 *
 * .. seealso::
 *    
 *    * :c:func:`tvwfree`: recommended method to free such a struct
 *
 * :param file: wrapper file structure to free
 */
void _tvwfreeparts(struct WRAPPER_FILE *file);

/**
 * Free all memory used by a wrapper file structure.
 *
 * This function recursively inspects and frees any non-NULL pointers in a
 * wrapper file, including its header, filename, metadata, and contents
 * buffers, and XML documents/errors.
 *
 * :param file: wrapper file structure to free
 */
void tvwfree(struct WRAPPER_FILE *file);

