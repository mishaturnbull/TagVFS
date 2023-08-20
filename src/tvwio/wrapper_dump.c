/* TagVFS wrapper file dump utility.  Compiles to own executable.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-17
 *
 * File info dump utility for TagVFS wrapper files.  This C file makes its own
 * executable product.
 */

#include "wrapper_dump.h"

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Invalid arguments!  Specify a file\n");
        return EINVAL;
    }

    WRAPPER_FILE *file = (WRAPPER_FILE*) calloc(1, sizeof(WRAPPER_FILE));
    int err = read_wrapper(argv[1], file);
    if (err != 0) {
        if (err == TV_NO_ERR) {
            // not a real issue
        } else if (err == TV_CHK_ERRNO) {
            fprintf(stderr, "Unable to perform operation: %d %s\n", 
                    err, strerror(err));
            return err;
        } else {
            // error, but probably one we can run with
            TV_LOGW("Issues detected in %s: code %d.  Continuing anyways!\n",
                    argv[1], err);
        }
    }

    printf("wf->filename = %s\n", file->filename);
    printf("wf*          = %p\n", (void*)file);
    printf("wf->fp       = %p\n", (void*)file->fp);
    printf("hdr->FV      = %u\n", file->header.format_version);
    printf("hdr->SOC GS  = %ju\n", file->header.start_of_contents_gs);
    printf("hdr->len     = %ju\n", file->header.len_of_contents);
    printf("hdr->CA meta = %u\n", file->header.comp_algo_meta);
    printf("hdr->CA file = %u\n", file->header.comp_algo_file);
    printf("hdr->sha512  = \n");
    for (int i = 0; i < 64; i++) {
        printf("%02x ", file->header.sha512[i]);
    }
    printf("\n");

    if (file->metadata == NULL) {
        printf("\nFailed to read metadata -- %d\n", err);
    } else {
        printf("\nRead %zu characters of metadata:\n", file->sizeof_meta);
        printf("%s\n", file->metadata);
        free(file->metadata);
    }
    if (file->xmlroot == NULL) {
        printf("Failed to parse XML -- xmlroot is NULL!\n");
        printf("Reported error: %s\n", file->xmlerr->message);
    } else {
        printf("wf->xmlroot = %p\n", (void*)file->xmlroot);
    }

    printf("\n");

    if (file->contents == NULL) {
        printf("Failed to read contents -- %d\n", err);
    } else {
        printf("Read %zu characters of contents:\n", file->sizeof_cont);
        printf("%s\n", file->contents);

        // don't check content's hash if it's invalid
        printf("\nCalculated sha512:\n");
        uint8_t *h2 = hash_of((uint8_t*)file->contents,
                file->header.len_of_contents);
        for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
            printf("%02x ", h2[i]);
        }
        printf("\n");

        if (cmp_hashes(file->header.sha512, h2) > 0) {
            printf("Contents hash match!\n");
        } else {
            printf("WARNING: File contents computed hash does not "
                    "match stored!\n");
        }
        free(h2);
        free(file->contents);
    }

    free(file);

    return 0;

}

