/* Types and constants for TVW files (wrappers).
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-19
 *
 * This file defines structs for handling TagVFS wrapper files.
 */

#pragma once

typedef struct {
    uint32_t format_version;
    uint64_t start_of_contents_gs;
    uint64_t len_of_contents;
    uint16_t comp_algo_meta;
    uint16_t comp_algo_file;
    // 512 "BIT" digest -- not 512 "byte".
    uint8_t sha512 [512 / 8];
} WRAPPER_FILE_HEADER;

typedef struct {
    FILE* fp;
    char* filename;
    WRAPPER_FILE_HEADER header;
} WRAPPER_FILE;

