/* TagVFS miscellaneous input/output utilities, for command-line scope.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-21
 *
 * This file implements some miscellaneous re-usable functions for interacting
 * with the user over stdin and stdout.
 */

#include "ioutils.h"

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

