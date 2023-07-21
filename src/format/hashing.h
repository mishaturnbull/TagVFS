/* TagVFS wrapper file hashing operations headers.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-19
 *
 * This file provides constants and declarations for wrapper file hash-related
 * operations.
 */

#pragma once

#include <stdio.h>
#include <openssl/sha.h>
#include "tvwrapper.h"
#include "tvfs_read.h"

int cmp_hashes(uint8_t *h1, uint8_t *h2);
uint8_t* hash_of_contents(WRAPPER_FILE *wrap);
