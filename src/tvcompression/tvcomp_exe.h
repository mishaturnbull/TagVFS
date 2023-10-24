/* TagVFS compression table dump header.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-19
 *
 * This file contains inclusions and declarations for the tvctool executable.
 */

#pragma once

#include <argp.h>
#include <stdio.h>

#include "version.h"
#include "debugs.h"
#include "errors.h"
#include "tvcompression.h"
#include "../tvmisc/ioutils.h"

int main(int argc, char **argv);

