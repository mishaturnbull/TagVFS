/* TagVFS unit testing general include header
 * AGPL-3.0
 * Initial rev 2023-10-29 Misha Turnbull
 *
 * Defines Criterion inclusions in one place for easy future changes.
 */

#pragma once

#include "config.h"

// make sure we've only got one api version selected
// it should be impossible to select both via menuconfig, but hand-editing of
// the .config file or config.h could do it
#if defined(CONFIG_CRITERION_API_V3) && defined(CONFIG_CRITERION_API_V2)
#error "Both v2 and v3 Criterion APIs are selected!  This is wrong!"
#endif

#ifdef CONFIG_CRITERION_API_V3
// criterion/new/assert.h seems to be missing a stdlib.h
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#endif

#ifdef CONFIG_CRITERION_API_V2
#warning "Using deprecated Criterion v2 assertion API!"
#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#endif

