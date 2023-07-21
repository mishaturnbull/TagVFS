/* TagVFS wrapper file dump utility.  Compiles to own executable.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-17
 *
 * File info dump utility for TagVFS wrapper files.  This C file makes its own
 * executable product.
 */

#include "wrapper_dump.h"

int main(int argc, char** argv) {
#ifdef CONFIG_DEBUG
    printf("Debug mode!  Woohoo!\n");
#endif
    if (argc <= 1) {
        printf("Invalid arguments!  Specify a file\n");
        return EINVAL;
    }

    WRAPPER_FILE *file = (WRAPPER_FILE*) calloc(1, sizeof(WRAPPER_FILE));
    int err = test_rd_file(argv[1], file);
    if (err != 0) {
        //return err;
        printf("It went really wrong! %d\n", err);
        return ENOENT;
    }

    printf("wf->filename = %s\n", file->filename);
    printf("wf*          = %p\n", (void*)file);
    printf("wf->fp       = %p\n", (void*)file->fp);
    printf("hdr->FV      = %u\n", file->header.format_version);
    printf("hdr->SOC GS  = %ju\n", file->header.start_of_contents_gs);
    printf("hdr->len     = %ju\n", file->header.len_of_contents);
    printf("hdr->CA meta = %u\n", file->header.comp_algo_meta);
    printf("hdr->CA file = %u\n", file->header.comp_algo_file);
    printf("hdr->sha 512 = \n");
    for (int i = 0; i < 64; i++) {
        printf("%02x ", file->header.sha512[i]);
    }
    printf("\n");

    printf("\nMetadata:\n");
    char *meta = calloc(sizeof(char), file->header.start_of_contents_gs - 88);
    err = read_metadata(file, meta);
    if (err != 0) {
        printf("Failed to read metadata -- %d\n", err);
    } else {
        printf("%s\n", meta);
    }
    free(meta);

    printf("\nContents:\n");
    char *cont = calloc(sizeof(char), file->header.len_of_contents);
    err = read_contents(file, cont);
    if (err != 0) {
        printf("Failed to read contents -- %d\n", err);
    } else {
        printf("%s\n", cont);

        // don't check content's hash if it's invalid
        printf("\nCalculated hash:\n");
        uint8_t *h2 = hash_of_contents(file);
        for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
            printf("%02x ", h2[i]);
        }
        printf("\n");

        if (cmp_hashes(file->header.sha512, h2) > 0) {
            printf("Contents hash match!\n");
        } else {
            printf("WARNING: File contents computed hash does not match stored!\n");
        }
        free(h2);
    }
    free(cont);

    fclose(file->fp);
    free(file);

    return 0;

}

