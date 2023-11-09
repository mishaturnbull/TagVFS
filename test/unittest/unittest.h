/* TagVFS unit testing general include header
 * AGPL-3.0
 * Initial rev 2023-10-29 Misha Turnbull
 *
 * Defines Criterion inclusions in one place for easy future changes.
 */

#pragma once

#include "config.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/parameterized.h>
#include <criterion/hooks.h>
#include <criterion/new/assert.h>


/**
 * Convert a constant string to a cr_malloc() string.
 *
 * Necessary because Criterion needs to control memory allocation during
 * parameterized test runs -- see
 * https://criterion.readthedocs.io/en/latest/parameterized.html#dynamically-allocating-parameters.
 */
char *crs(const char *str, size_t sz);

