/* TagVFS miscellaneous input/output utilities headers.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-21
 *
 * This file defines/declares  some miscellaneous re-usable functions for
 * interacting with the user over stdin and stdout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

size_t read_stdin_input(char* prompt, char* buf, size_t maxsize);

