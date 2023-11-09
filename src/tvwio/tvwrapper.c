/* Utility functions for working with tagvfs wrapper files.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-08-20
 *
 * This file implements some utility functions for working with tv wrapper
 * files and their struct representations in memory.
 */

#include "tvwrapper.h"

void _tvwfreeparts(struct WRAPPER_FILE *file) {
    if (file->fp != NULL) {
        fclose(file->fp);
        file->fp = NULL;
    }

    if (file->filename != NULL) {
        free(file->filename);
        file->filename = NULL;
    }

    if (file->metadata != NULL) {
        free(file->metadata);
        file->metadata = NULL;
    }

    if (file->xmlroot != NULL) {
        xmlFreeDoc(file->xmlroot);
        file->xmlroot = NULL;
    }

    if (file->xmlerr != NULL) {
        /**
         * .. TODO::
         *    confirm how to free an xmlErrPtr
         */
        xmlResetError(file->xmlerr);
        file->xmlerr = NULL;
    }

    if (file->contents != NULL) {
        free(file->contents);
        file->contents = NULL;
    }

}

void tvwfree(struct WRAPPER_FILE *file) {
    _tvwfreeparts(file);
    free(file);
}

