/* TagVFS wrapper file format reader.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-13
 *
 * Defines structs & functions for reading TagVFS wrapper format files.
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <endian.h>
#include "tvwrapper.h"
#include "hashing.h"

int test_rd_file(char *filename, WRAPPER_FILE *out);
int test_rd_hdr(WRAPPER_FILE *wrap);
int read_metadata(WRAPPER_FILE *wrap, char *buf);
int read_contents(WRAPPER_FILE *wrap, char *buf);

