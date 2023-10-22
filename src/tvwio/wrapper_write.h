/* Wrapper-file creation headers & declarations.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-23
 *
 * This file contains inclusions, defines, and declarations for the CLI utility
 * to create TagVFS wrapper files.
 */

#pragma once

#include <argp.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "version.h"
#include "tvwrapper.h"
#include "tvw_o.h"
#include "errors.h"
#include "debugs.h"
#include "../tvmisc/ioutils.h"

//size_t read_stdin_input(char* prompt, char* buf, size_t maxsize);
int main(int argc, char** argv);

