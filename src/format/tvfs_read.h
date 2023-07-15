/* TagVFS wrapper file format reader.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-13
 *
 * Defines structs & functions for reading TagVFS wrapper format files.
 */

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

typedef struct {
    uint32_t format_version;
    uint64_t start_of_contents_gs;
    uint64_t len_of_contents;
    uint16_t comp_algo_meta;
    uint16_t comp_algo_file;
    uint8_t* sha512;
} WRAPPER_FILE_HEADER;

typedef struct {
    FILE* fp;
    char* filename;
    WRAPPER_FILE_HEADER header;
} WRAPPER_FILE;

int test_rd_file(char *filename, WRAPPER_FILE *out);
int main(int argc, char **argv);

