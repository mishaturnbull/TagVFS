/* Implementation of the wrapper-file creation utility.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-23
 *
 * This file implements the sourcecode for a CLI utility to create a TagVFS
 * wrapper file.
 */

#include "wrapper_write.h"

size_t read_stdin_input(char* prompt, char* buf, size_t maxsize) {
    printf(prompt);

    // don't allow a line longer than maxsize
    size_t maxlinesize = maxsize > CONFIG_TVWMAKE_MAX_LINE_SIZE ?
        maxsize : CONFIG_TVWMAKE_MAX_LINE_SIZE;

    char* line = calloc(maxlinesize, sizeof(char));

    while (fgets(line, maxlinesize, stdin) != NULL) {
        strncat(buf, line, maxlinesize);
    }

    // clear EOF/sticky from stdin so if this function (or something like it)
    // gets called after us it doesn't just instantly die on the EOF left
    // behind
    clearerr(stdin);

    return strlen(buf);
}


/* arguments:
 * cam
 * caf
 * output file name
 * input file name (optional)
 */
int main(int argc, char** argv) {

    if (argc != 4 && argc != 5) {
        printf("invalid arguments\n");
        return 1;
    }

    uint16_t cam = (uint16_t)strtol(argv[1], NULL, 10);
    uint16_t caf = (uint16_t)strtol(argv[2], NULL, 10);

    struct WRAPPER_FILE *file = (struct WRAPPER_FILE*) calloc(1, sizeof(struct WRAPPER_FILE));
    file->header.format_version = CONFIG_FORMAT_VERSION;
    file->header.comp_algo_meta = cam;
    file->header.comp_algo_file = caf;
    file->filename = argv[3];

    // read metadata from standard input
    char* buf = calloc(CONFIG_TVWMAKE_MAX_META_SIZE, sizeof(char));
    file->sizeof_meta = read_stdin_input("Enter metadata >\n", buf, 64);
    file->metadata = calloc(file->sizeof_cont, sizeof(char));
    memcpy(file->metadata, buf, file->sizeof_meta);
    free(buf);
    TV_LOGI("\nAccepted %zu bytes from stdin\n", file->sizeof_meta);

    // read the file contents
    // if we got 4 args, prompt user for multiline input.
    // if we got 5 args, open the specified file
    if (argc == 4) {
        buf = calloc(CONFIG_TVWMAKE_MAX_FILE_SIZE, sizeof(char));
        file->sizeof_cont = read_stdin_input("Enter file contents >\n", buf, 64);
        file->contents = calloc(file->sizeof_cont, sizeof(char));
        memcpy(file->contents, buf, file->sizeof_cont);
        free(buf);
        TV_LOGI("\nAccepted %zu bytes from stdin\n", file->sizeof_cont);
    } else {
        // argc == 5
        FILE* fp = fopen(argv[4], "r");
        if (fp == NULL) {
            TV_LOGE("\nfailed to read %s -- errno = %d\n", argv[4], errno);
            free(file);
            return errno;
        }
        fseek(fp, 0, SEEK_END);
        size_t len_file = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        file->sizeof_cont = len_file;
        file->contents = calloc(file->sizeof_cont, sizeof(char));
        size_t nread = fread(file->contents, sizeof(char), file->sizeof_cont,
                fp);
        if (nread != len_file) {
            TV_LOGE("\nfailed to read %s -- only got %zu bytes, expected %zu\n",
                    argv[4], nread, len_file);
            free(file->contents);
            free(file);
            return TV_CHK_ERRNO;
        }
        TV_LOGI("\nRead %zu bytes from %s\n", nread, argv[4]);
    }

    // header: done.  meta: done.  file: done.
    // time to write!
    int res = tvw_write(file);

    return res;
}

