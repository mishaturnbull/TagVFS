/* TagVFS compression table dump utility
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-19
 */

#include "tvcomp_exe.h"

static char doc[] = "Interact with TagVFS compression library.";
static char args_doc[] = "";

static struct argp_option options[] = {
    {"print-table", 'p', 0, 0, "Print compression algo table", 0},

    {"compress", 'c', 0, 0, "Compress data", 0},
    {"extract", 'x', 0, 0, "Extract (decompress) data", 0},

    {0, 0, 0, 0, "The following options are common to de/compress commands:", 0},
    {"algo", 'a', "ALGO", 0, "Use ALGO algorithm to de/compress data", 0},
    {"input", 'i', "FILE", 0, "Input from FILE instead of standard input", 0},
    {"output", 'o', "FILE", 0, "Output to FILE instead of standard output", 0},

    {0, 0, 0, 0, 0, 0}
};

struct arguments {
    int do_print;
    int do_comp;
    int do_decomp;

    uint16_t algo;
    char* infilename;
    char* outfilename;
};

static int parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'p':
            arguments->do_print = 1;
            break;
        case 'c':
            arguments->do_comp = 1;
            break;
        case 'x':
            arguments->do_decomp = 1;
            break;
        case 'a':
            arguments->algo = (uint16_t)strtol(arg, NULL, 10);
            break;
        case 'i':
            arguments->infilename = arg;
            break;
        case 'o':
            arguments->outfilename = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char **argv) {
    struct arguments arguments;
    arguments.do_print = 0;
    arguments.do_comp = 0;
    arguments.do_decomp = 0;
    arguments.algo = 0;
    arguments.infilename = NULL;
    arguments.outfilename = NULL;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.do_print + arguments.do_comp + arguments.do_decomp != 1) {
        fprintf(stderr, 
                "ERROR: exactly one of -p, -c, or -x must be specified\n");
        return 1;
    }

    if (arguments.do_print) {
        size_t outsize;
        char *outbuf;
        tvcomp_make_table(&outsize, &outbuf);

        printf(outbuf);
        return 0;
    }

    // we're doing an operation!  woooo!
    struct COMPRESSION_ALGO selected = COMP_TABLE[arguments.algo];
    int (*operation)(size_t*, char**, size_t*, char**);
    if (arguments.do_comp) {
        operation = selected.compress;
    } else if (arguments.do_decomp) {
        operation = selected.decompress;
    }

    char *preop;
    size_t nread;
    if (arguments.infilename == NULL) {
        preop = calloc(CONFIG_TVCTOOL_MAX_CLI_INPUT_SIZE, sizeof(char));
        nread = read_stdin_input("Enter file contents > \n", preop, 64);
        //realloc(preop, nread);
        //TV_LOGV("\nAccepted %zu bytes from stdin\n", nread);
    } else {
        FILE *ifp = fopen(arguments.infilename, "r");
        if (ifp == NULL) {
            //TV_LOGE("\nfailed to read %s -- errno = %d\n", arguments.infilename,
            //        errno);
            return errno;
        }
        fseek(ifp, 0, SEEK_END);
        size_t len_file = ftell(ifp);
        fseek(ifp, 0, SEEK_SET);
        preop = calloc(len_file, sizeof(char));
        nread = fread(preop, sizeof(char), len_file, ifp);
        fclose(ifp);
    }

    char *postop;
    size_t postsize;
    operation(&nread, &preop, &postsize, &postop);

    if (arguments.outfilename == NULL) {
        puts(postop);
    } else {
        FILE *ofp = fopen(arguments.outfilename, "wb");
        if (ofp == NULL) {
            return errno;
        }
        size_t nwrote = fwrite(
                postop,
                sizeof(char),
                postsize,
                ofp);
        fclose(ofp);
    }


    return 0;
}


