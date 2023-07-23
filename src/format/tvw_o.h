/* TagVFS wrapper file format writer.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-21
 *
 * Defines structs & functions for writing TagVFS wrapper format files.
 */

#pragma once

#include "tvwrapper.h"
#include "hashing.h"

int tvw_write(WRAPPER_FILE *wrap);
int tvw_upd_hdr_from_conts(WRAPPER_FILE *wrap);
int tvw_write_hdr(WRAPPER_FILE *wrap);

