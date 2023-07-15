/* TagVFS wrapper file format reader.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-13
 *
 * Implements functions for reading TagVFS wrapper format files.
 */

#include "tvfs_read.h"

int test_rd_file(char* filename, WRAPPER_FILE *out) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        // something went wrong
        int errsv = errno;
        fprintf(stderr, "Couldn't open %s: error=%d -> %s\n", 
                filename, errsv, strerror(errsv));
        return errsv;
    }

    printf("Opened %s: %d\n", filename, fp);

    out->filename = filename;
    out->fp = fp;

    return 0;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Invalid arguments!  Specify a file\n");
        return EINVAL;
    }

    WRAPPER_FILE file;
    int err = test_rd_file(argv[1], &file);
    if (err != 0) {
        return err;
    }

    printf("wf*          = %d\n", file);
    printf("wf->filename = %s\n", file.filename);
    printf("wf->fp       = %d\n", file.fp);
    fclose(file.fp);

    return 0;

}

