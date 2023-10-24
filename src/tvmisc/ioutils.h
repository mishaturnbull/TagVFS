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

/**
 * Provides a general-purpose function to read multi-line user input from stdin.
 *
 * When called, prints the supplied prompt, then reads data from the user into the supplied buffer (up to maxsize) until the user sends a ctrl-D (EOF).
 *
 * :param prompt: string to print before reading data.  Convention indicates
 *    this should usually end in a newline.
 * :param buf: space to put the user data (output parameter).  This must be
 *    ALLOCATED AND INITALIZED by the **caller**!
 * :param maxsize: the maximum length to allow any one line to be
 */
size_t read_stdin_input(char* prompt, char* buf, size_t maxsize);

