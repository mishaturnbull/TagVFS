/* TagVFS wrapper file hashing operations and checking.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-19
 *
 * This file provides hash-related functions for wrapper files.
 */

#include "hashing.h"

int cmp_hashes(uint8_t *h1, uint8_t *h2) {
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        if (h1[i] != h2[i]) {
            return 0;
        }
    }

    return 1;
}

uint8_t* hash_of(uint8_t *in, size_t size) {
    uint8_t *buf = calloc(SHA512_DIGEST_LENGTH, sizeof(uint8_t));
    // OpenSSL crypto library call
    SHA512(in, size, buf);
    return buf;
}

