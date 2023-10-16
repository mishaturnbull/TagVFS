/* Implementation of the wrapper-file creation utility.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-23
 *
 * This file implements the sourcecode for a CLI utility to create a TagVFS
 * wrapper file.
 */

#include "wrapper_write.h"

static char doc[] = "Create a TVW wrapper file.";
static char args_doc[] = "OUTFILE [INFILE]";

// no flag options in use
static struct argp_option options[] = {
    {"comp-meta", 'm', "COMP-META", 0, 
        "Select compression algorithm for metadata", 0},
    {"comp-file", 'f', "COMP-FILE", 0, 
        "Select compression algorithm for file contents", 0},
    {0, 0, 0, 0, 0, 0}
};

struct arguments {
    uint16_t cam;
    uint16_t caf;
    char* outfile;
    char* infile;
};

static int parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'm':
            arguments->cam = (uint16_t)strtol(arg, NULL, 10);
            break;
        case 'f':
            arguments->caf = (uint16_t)strtol(arg, NULL, 10);
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num == 0) {
                arguments->outfile = arg;
            } else if (state->arg_num == 1) {
                arguments->infile = arg;
            } else {
                return ARGP_ERR_UNKNOWN;
            }
            break;
        case ARGP_KEY_END:
            if (state->arg_num == 1) {
                arguments->infile = NULL;
            } else if (state->arg_num == 2) {
                // that's okay.  we got a filename already by this point
            } else {
                argp_usage(state);
                return ARGP_ERR_UNKNOWN;
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

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

int main(int argc, char** argv) {

    struct arguments arguments;
    arguments.cam = 0;
    arguments.caf = 0;
    arguments.outfile = NULL;
    arguments.infile = NULL;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    uint16_t cam = arguments.cam;
    uint16_t caf = arguments.caf;

    struct WRAPPER_FILE *file = (struct WRAPPER_FILE*) calloc(1, sizeof(struct WRAPPER_FILE));
    file->header.format_version = CONFIG_FORMAT_VERSION;
    file->header.comp_algo_meta = cam;
    file->header.comp_algo_file = caf;
    file->filename = arguments.outfile;

    // read metadata from standard input
    char* buf = calloc(CONFIG_TVWMAKE_MAX_META_SIZE, sizeof(char));
    file->sizeof_meta = read_stdin_input("Enter metadata >\n", buf, 64);
    file->metadata = calloc(file->sizeof_cont, sizeof(char));
    memcpy(file->metadata, buf, file->sizeof_meta);
    free(buf);
    TV_LOGI("\nAccepted %zu bytes from stdin\n", file->sizeof_meta);

    // read the file contents
    if (arguments.infile == NULL) {
        buf = calloc(CONFIG_TVWMAKE_MAX_FILE_SIZE, sizeof(char));
        file->sizeof_cont = read_stdin_input("Enter file contents >\n", buf, 64);
        file->contents = calloc(file->sizeof_cont, sizeof(char));
        memcpy(file->contents, buf, file->sizeof_cont);
        free(buf);
        TV_LOGI("\nAccepted %zu bytes from stdin\n", file->sizeof_cont);
    } else {
        // we were provided a filename in arguments.infile
        FILE* fp = fopen(arguments.infile, "r");
        if (fp == NULL) {
            TV_LOGE("\nfailed to read %s -- errno = %d\n", arguments.infile, 
                    errno);
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
                    arguments.infile, nread, len_file);
            free(file->contents);
            free(file);
            return TV_CHK_ERRNO;
        }
        TV_LOGI("\nRead %zu bytes from %s\n", nread, arguments.infile);
    }

    // header: done.  meta: done.  file: done.
    // time to write!
    int res = tvw_write(file);

    return res;
}

