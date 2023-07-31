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
#include "tvw_i.h"

/**
 * Compare two hashes, and return a boolean indicator of whether they are the
 * same.  This is a "short-circuit" function; it will stop checking hashes
 * further after any difference is found.
 *
 * :param h1: Hash #1.  Must be exactly 64 elements long (each element =
 *     uint8_t); otherwise behavior is undefined.
 * :param h2: Hash #2.  Must be exactly 64 elements long (each element =
 *     uint8_t); otherwise behavior is undefined.
 * :return: 1 if hashes are the same; 0 otherwise.
 */
int cmp_hashes(uint8_t *h1, uint8_t *h2);

/**
 * Compute a SHA512 hash of a given buffer and return a pointer to it's
 * containing uint8_t array.  This more or less directly calls the OpenSSL
 * SHA512 function.
 *
 * :param in: Pointer to the buffer of data to hash.
 * :param size: Size of the ``in`` parameter (number of elements)
 * :return: Pointer to buffer containing the hash digest.  Always exactly 64
 *    elements long.  This memory must be freed by the caller when no longer
 *    used.
 */
uint8_t* hash_of(uint8_t *in, size_t size);

