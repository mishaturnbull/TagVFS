/* TagVFS  compression algo header: passthru (no compression)
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-10-17
 *
 * Header for the passthru (no) compression algo.
 */

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int passthru_func(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

int effectivity_test_comp(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);
int effectivity_test_decomp(
        size_t *insize,
        char **inbuf,
        size_t *outsize,
        char **outbuf);

