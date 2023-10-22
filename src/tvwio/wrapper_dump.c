/* TagVFS wrapper file dump utility.  Compiles to own executable.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-17
 *
 * File info dump utility for TagVFS wrapper files.  This C file makes its own
 * executable product.
 */

#include "wrapper_dump.h"

static char doc[] = "Dump a TVW wrapper file.";
static char args_doc[] = "FILENAME";

static struct argp_option options[] = {
    {"print-xml", 'x', 0, OPTION_ARG_OPTIONAL, "Print raw XML metadata", 0},
    {0, 0, 0, 0, 0, 0}
};

struct arguments {
    int print_xml;
    char* filename;
};

static int parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'x':
            arguments->print_xml = 1;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1) {
                // too many args (more than 1)
                return ARGP_ERR_UNKNOWN;
            }
            arguments->filename = arg;
            break;
        case ARGP_KEY_END:
            if (arguments->filename == NULL) {
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

static void print_element_names(int level, xmlNode *a_node) {
    xmlNode *cur_node = NULL;
    for (cur_node=a_node; cur_node; cur_node=cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            for (int i = 0; i < level; i++) {
                printf("| ");
            }
            if (!xmlIsBlankNode(cur_node->children)) {
                xmlChar *cont = cur_node->children->content;
                printf("%s = %s\n", cur_node->name, cont);
            } else {
                printf("%s:\n", cur_node->name);
            }
        }
        print_element_names(level + 1, cur_node->children);
    }
}

int main(int argc, char** argv) {
    struct arguments arguments;
    arguments.print_xml = 0;
    arguments.filename = NULL;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    struct WRAPPER_FILE *file = (struct WRAPPER_FILE*) calloc(1,
            sizeof(struct WRAPPER_FILE));
    int err = read_wrapper(arguments.filename, file);
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
                    arguments.filename, err);
        }
    }

    printf("wf->filename = %s\n", file->filename);
    printf("wf*          = %p\n", (void*)file);
    printf("wf->fp       = %p\n", (void*)file->fp);
    printf("hdr->FV      = %u\n", file->header.format_version);
    printf("hdr->SOC GS  = %ju\n", file->header.start_of_contents_gs);
    printf("hdr->len     = %ju\n", file->header.len_of_contents);
    printf("hdr->CA meta = %u ", file->header.comp_algo_meta);
    printf("(%s)\n", COMP_TABLE[file->header.comp_algo_meta].human_name);
    printf("hdr->CA file = %u ", file->header.comp_algo_file);
    printf("(%s)\n", COMP_TABLE[file->header.comp_algo_file].human_name);
    printf("hdr->sha512  = \n");
    for (int i = 0; i < 64; i++) {
        printf("%02x ", file->header.sha512[i]);
    }
    printf("\n\n");

    if (file->metadata == NULL) {
        printf("\nFailed to read metadata -- %d\n", err);
    } else {
        printf("\nRead %zu characters of metadata\n", file->sizeof_meta);
        if (arguments.print_xml) {
            printf("%s\n", file->metadata);
        }
    }
    if (file->xmlroot == NULL) {
        printf("Failed to parse XML -- xmlroot is NULL!\n");
        printf("xmlerr = %p: %s", (void*)(file->xmlerr), file->xmlerr->message);
    } else {
        printf("wf->xmlroot = %p\n", (void*)file->xmlroot);
        print_element_names(0, xmlDocGetRootElement(file->xmlroot));
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
    }

    tvwfree(file);

    return 0;

}

